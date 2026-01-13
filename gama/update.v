module gama

import os

pub fn (p Project) update_toolchain(inst Installation) ! {
	p.copy_build_native_artifacts(inst, true)!
	p.copy_build_web_artifacts(inst, true)!
	os.rmdir_all(os.join_path(p.path, 'include'))!
	inst.copy_gama(os.join_path(p.path, 'include'), true) or {
		return error('Failed to update gama source files')
	}
}
