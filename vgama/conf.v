module vgama

import os
import toml
import rand

pub struct ProjectGamaConf {
pub:
	version  Version @[required]
	compiler string
}

pub struct ProjectConf {
pub:
	name        string @[required]
	uuid        string @[required]
	description string  = 'A sample gama projec'
	version     Version = Version{}
	author      string  = 'gama'

	gama ProjectGamaConf
}

pub fn (c ProjectConf) substitute(templ string) string {
	mut code := templ

	subs := {
		'name':          c.name
		'description':   c.description
		'version':       c.version.str()
		'uuid':          c.uuid
		'author':        c.author
		'gama.version':  c.gama.version.str()
		'gama.compiler': c.gama.compiler
	}
	for key, val in subs {
		code = code.replace('{{' + key + '}}', val)
	}
	return code
}

pub fn (c ProjectConf) save(path string) ! {
	os.write_file(path, '
uuid = "${c.uuid}"
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
	mut uuid := doc.value('uuid').string()
	if uuid == '' {
		uuid = rand.uuid_v7()
	}
	return ProjectConf{
		name:        doc.value('name').string()
		description: doc.value('description').string()
		version:     Version.parse(doc.value('version').string())!
		author:      doc.value('author').string()
		uuid:        uuid
		gama:        ProjectGamaConf{
			version:  Version.parse(doc.value('gama.version').string())!
			compiler: doc.value('gama.compiler').string()
		}
	}
}
