module vgama

import os

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
