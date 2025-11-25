module vgama

import os

pub struct Installation {
pub:
	lib       string @[required]
	templates string @[required]
	runners   string @[required]
}

pub fn Installation.folder(repo string) Installation {
	return Installation{
		lib:       os.join_path(repo, 'lib')
		templates: os.join_path(repo, 'templates')
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

pub fn (i Installation) get_gama_version() !Version {
	parts := os.read_file(os.join_path(i.lib, 'gama.h')) or {
		return error('Could not read gama.h at ${i.lib} file from installation')
	}.split('\n').filter(|s| s.starts_with('#define GAMA_VERSION')).map(|s| u16(s.split(' ')#[-1..][0].int()))
	if parts.len != 3 {
		return error('Could not read gama version from gama.h')
	}
	return Version{parts[0], parts[1], parts[2]}
}

pub fn (i Installation) copy_gama(dest string, overwrite bool) ! {
	os.cp_all(i.lib, dest, overwrite) or { return error('Error copying gama') }
}
