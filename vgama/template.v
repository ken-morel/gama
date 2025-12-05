module vgama

import os

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

pub fn GamaTemplate.get_description(path string) !string {
	return os.read_file(os.join_path(path, 'README.md')) or {
		return error('Error reading readme from template: ${err}')
	}.split('\n').filter(|v| v != '' && !v.starts_with('#'))[0]
}
