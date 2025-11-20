module vgama

import os
import toml

struct ZigCC {
	exepath string
}

fn (z ZigCC) build_shared(file string, include string, name string, out string) !string {
	ext := $if windows {
		'dll'
	} $else $if darwin {
		'dylib'
	} $else {
		'so'
	}
	output := os.join_path(out, '${name}.${ext}')
	compile_cmd := '${z.exepath} cc -shared -o ${output} ${file} -I${include}'
	res := os.execute(compile_cmd)

	return if res.exit_code != 0 {
		error('Failed to compile code: ${res.output}')
	} else {
		output
	}
}

pub struct GamaTemplate {
pub:
	name        string @[required]
	path        string @[required]
	description string
}

pub fn (g GamaTemplate) copy_to(dest string) ! {
	os.cp_all(g.path, dest, false) or {
		return error('Error copying files from ${g.path} to ${dest}: ${err}')
	}
}

pub struct Installation {
pub:
	lib       string @[required]
	templates string @[required]
	zig       string @[required]
	runners   string @[required]
}

pub fn (i Installation) get_zig() ZigCC {
	return ZigCC{i.zig}
}

pub fn GamaTemplate.get_description(path string) !string {
	return os.read_file(os.join_path(path, 'README.md')) or {
		return error('Error reading readme from template: ${err}')
	}.split('\n').filter(|v| v != '' && v.starts_with('#'))[0]
}

pub fn Installation.dev(repo string) Installation {
	return Installation{
		lib:       os.join_path(repo, 'lib')
		templates: os.join_path(repo, 'templates')
		zig:       'zig'
		runners:   os.join_path(repo, 'runners')
	}
}

pub fn (i Installation) get_templates() ![]GamaTemplate {
	names := os.ls(i.templates) or { return error('Error reading gama templates folder: ${err}') }
	mut templates := []GamaTemplate{cap: names.len}
	for name in names {
		path := os.join_path(i.templates, name)
		desc := GamaTemplate.get_description(path) or { 'no description(${err})' }
		templates << GamaTemplate{
			name:        name
			path:        path
			description: desc
		}
	}
	return templates
}

pub struct Version {
pub:
	major u16
	minor u16 = 1
	patch u16
}

pub fn (v Version) str() string {
	return '${v.major}.${v.minor}.${v.patch}'
}

pub fn Version.parse(txt string) !Version {
	start := if txt[0] == `v` { 1 } else { 0 }
	parts := txt[start..].split('.').map(|s| u16(s.int()))
	return Version{parts[0], parts[1], parts[2]}
}

pub fn (i Installation) get_gama_version() !Version {
	parts := os.read_file(os.join_path(i.lib, 'gama.h')) or {
		return error('Could not read gama.h at ${i.lib} file from installation')
	}.split('\n').filter(|s| s.starts_with('#define GAMA_VERSION')).map(|s| u16(s.split(' ')#[-1..][0].int()))
	if parts.len != 3 {
		return error('Could not read gama version from gama.h')
	}
	return Version{parts[0], parts[1], parts[2]}
}

pub struct Project {
pub:
	path string @[required]
}

pub fn (p Project) build_shared(z ZigCC, out string) ! {
	conf := p.get_conf() or { return error('Error getting project configuration: ${err}') }

	z.build_shared(os.join_path(p.path, 'src', 'main.c'), os.join_path(p.path, 'gama'),
		conf.name, out) or { return err }
}

pub fn (p Project) build_path(type string) string {
	return os.join_path(p.path, 'build', type)
}

pub fn Project.find_at(start string) ?Project {
	mut project_root := start
	for {
		if os.exists(os.join_path(project_root, 'gama.toml')) {
			break
		}
		parent := os.dir(project_root)
		if parent == project_root {
			return none
		}
		project_root = parent
	}
	return Project{project_root}
}

pub fn (p Project) get_conf() !ProjectConf {
	return ProjectConf.load(os.join_path(p.path, 'gama.toml'))
}

pub fn (p Project) set_conf(conf ProjectConf) ! {
	conf.save(os.join_path(p.path, 'gama.toml'))!
}

pub fn (p Project) add_editor_config() ! {
	os.write_file(os.join_path(p.path, '.clangd'), '
CompileFlags:
  Add:
    - -I/home/engon/gama/lib
---
If:
  PathMatch: .*\\.h\$
CompileFlags:
  Add:
    - -x
    - c
')!
}

pub fn (i Installation) copy_gama(dest string, overwrite bool) ! {
	os.cp_all(i.lib, dest, overwrite) or { return error('Error copying gama') }
}

pub fn Project.generate(inst Installation, conf ProjectConf, template GamaTemplate) !Project {
	project_dir := conf.name
	os.mkdir(project_dir)!

	os.mkdir(os.join_path(project_dir, 'assets'))!

	template.copy_to(project_dir)!

	inst.copy_gama(os.join_path(project_dir, 'gama'), false)!

	project := Project{
		path: os.abs_path(project_dir)
	}
	project.set_conf(conf)!
	project.add_editor_config()!
	return project
}

pub struct ProjectGamaConf {
pub:
	version Version @[required]
}

pub struct ProjectConf {
pub:
	name        string @[required]
	description string  = 'A sample gama projec'
	version     Version = Version{}
	repo        string

	gama ProjectGamaConf
}

pub fn (c ProjectConf) save(path string) ! {
	os.write_file(path, '
name = "${c.name}"
description = "${c.description.replace('"',
		'"')}"
version = "${c.version.str()}"
repo = "${c.repo}"

[gama]

version = "${c.gama.version.str()}"
		')!
	return
}

pub fn ProjectConf.load(path string) !ProjectConf {
	doc := toml.parse_file(path)!
	return ProjectConf{
		name:        doc.value('name').string()
		description: doc.value('description').string()
		version:     Version.parse(doc.value('version').string())!
		repo:        doc.value('repo').string()
		gama:        ProjectGamaConf{
			version: Version.parse(doc.value('gama.version').string())!
		}
	}
}

pub fn (p Project) build_native(inst Installation) ! {
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	// TODO: This path should eventually be '${$os}_${$arch}'
	runner_path := os.join_path(inst.runners, 'native')

	if !os.exists(runner_path) {
		return error('no pre-compiled native runner found at ${runner_path}')
	}
	os.cp_all(runner_path, build_dir, false) or { return error('failed to copy runner: ${err}') }
	p.build_shared(inst.get_zig(), os.join_path('build', 'native'))!
}

pub fn (p Project) reset_gama(inst Installation) ! {
	inst.copy_gama(os.join_path(p.path, 'gama'), true)!
}
