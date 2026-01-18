module gama

import os

pub fn (p Project) update_toolchain(inst Installation) ! {
	p.copy_build_native_artifacts(inst, true)!
	p.copy_build_web_artifacts(inst, true)!
	lib := os.join_path(p.path, 'include')
	os.rmdir_all(lib)!
	os.mkdir_all(lib) or {}
	inst.copy_gama(lib, true) or { return error('Failed to update gama source files') }
}
