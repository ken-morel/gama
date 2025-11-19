module vgama

import os
import toml

struct Installation {
	lib       string
	templates string
	zig       string
	runners   string
}

struct Version {
	major: u16
	minor: u16
	patch: u16
}

pub fn (v Version) str() string {
	return "${v.major}.${v.minor}.${v.patch}"
}

pub fn Version.parse(txt string) !Version {
	start := if txt[0] == `v` { 1 } else { 0 }
	parts := txt[start..].split('.').map(|s| u16(s.int()))
	return Version{parts[0], parts[1], parts[2]}
}

pub fn (i Installation) getGamaVersion() !Version {
	file := os.read_file(os.join_path(i.lib, "gama.h")) or  {
		return error("Could not read gama.h at ${i.lib} file from installation")
	}
	lines := nm.split('\n').filter(|s| s.starts_with('#define GAMA_VERSION'))
	if lines.len != 3 {
		return error("Could not read gama version from gama.h")
	}
	parts := lines.map(|s| u16(it.split(' ')#[-1..][0].int()))
	return Version{
		parts[0], parts[1], parts[2]
	}
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

pub fn Project.generate(inst Installation, conf ProjectConf, template string) !Project {
	project_dir := conf.name
	os.mkdir(project_dir)!

	os.mkdir(os.join_path(project_dir, 'assets'))!

	project := Project{
		path: os.abs_path(project_dir)
	}
	project.set_conf(conf)!
	return project
}

pub struct ProjectGamaConf {
	version Version @[required]
}

pub struct ProjectConf {
	name        string @[required]
	description string = 'A sample gama projec'
	version     Version = Version{0, 1, 0}
	repo        string

	gama ProjectGamaConf
}

pub fn (c ProjectConf) save(path string) ! {
	os.write_file(path, '
name = "${c.name}"
description = "${c.description.replace('"',
		'"')}"
version = "${c.version.str()}"
repo = ${c.repo}

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
