module vgama

import os

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

pub fn executable_extension(f string) string {
	return $if windows {
		'${f}.exe'
	} $else {
		f
	}
}

fn get_static_library_extension() string {
	return $if windows {
		'lib'
	} $else {
		'a'
	}
}

pub fn static_libvgama_name() string {
	return 'libvgama.${get_static_library_extension()}'
}
