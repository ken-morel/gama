module vgama

import os
import toml

pub struct ProjectGamaConf {
pub:
	version  Version @[required]
	compiler string
}

pub struct ProjectConf {
pub:
	name        string @[required]
	description string  = 'A sample gama projec'
	version     Version = Version{}
	author      string  = 'gama'

	gama ProjectGamaConf
}

pub fn (c ProjectConf) save(path string) ! {
	os.write_file(path, '
name = "${c.name}"
description = "${c.description.replace('"',
		'"')}"
version = "${c.version.str()}"
author = "${c.author}"

[gama]

version = "${c.gama.version.str()}"
compiler = "${c.gama.compiler}"
		')!
	return
}

pub fn ProjectConf.load(path string) !ProjectConf {
	doc := toml.parse_file(path)!
	return ProjectConf{
		name:        doc.value('name').string()
		description: doc.value('description').string()
		version:     Version.parse(doc.value('version').string())!
		author:      doc.value('author').string()
		gama:        ProjectGamaConf{
			version:  Version.parse(doc.value('gama.version').string())!
			compiler: doc.value('gama.compiler').string()
		}
	}
}
