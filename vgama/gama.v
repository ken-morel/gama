module vgama

import os
import toml

pub struct Installation {
pub:
	lib       string @[required]
	templates string @[required]
	zig       string @[required]
	runners   string @[required]
}

pub struct GamaTemplate {
pub:
	name        string @[required]
	path        string @[required]
	description string
}

pub fn (g GamaTemplate) copy_to(dest string) ! {
	return os.cp_all(g.path, dest, false)
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
	path string @[required]
}

pub fn (p Project) get_conf() !ProjectConf {
	return ProjectConf.load(os.join_path(p.path, 'gama.toml'))
}

pub fn (p Project) set_conf(conf ProjectConf) ! {
	conf.save(os.join_path(p.path, 'gama.toml'))!
}

pub fn Project.generate(inst Installation, conf ProjectConf, template GamaTemplate) !Project {
	project_dir := conf.name
	os.mkdir(project_dir)!

	os.mkdir(os.join_path(project_dir, 'assets'))!

	project := Project{
		path: os.abs_path(project_dir)
	}
	project.set_conf(conf)!
	template.copy_to('.')!
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
