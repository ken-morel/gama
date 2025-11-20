module vgama

import os
import toml

pub fn (p Project) reset_gama(inst Installation) ! {
	inst.copy_gama(os.join_path(p.path, 'gama'), true)!
}

fn get_shared_library_extension() string {
	return $if windows {
		'dll'
	} $else $if darwin {
		'dylib'
	} $else {
		'so'
	}
}

fn executable_extension(f string) string {
	return $if windows {
		'${f}.exe'
	} $else {
		f
	}
}

pub fn libgama_name() string {
	return 'libgama.${get_shared_library_extension()}'
}
