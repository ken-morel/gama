module vgama

import os

pub fn (p Project) reset_gama(inst Installation) ! {
	inst.copy_gama(os.join_path(p.path, 'include'), true)!
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

pub fn executable_extension(f string) string {
	return $if windows {
		'${f}.exe'
	} $else {
		f
	}
}

pub fn libvgama_name() string {
	return 'libvgama.${get_shared_library_extension()}'
}
