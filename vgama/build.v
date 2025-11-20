module vgama

import os

pub fn (p Project) build_native(inst Installation) ! {
	conf := p.get_conf()
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	// TODO: This path should eventually be '${$os}_${$arch}'
	runner_path := os.join_path(inst.runners, 'native')
	if !os.exists(runner_path) {
		return error('no pre-compiled native runner found at ${runner_path}')
	}
	os.cp_all(runner_path, build_dir, false) or { return error('failed to copy runner: ${err}') }

	libgama_path := os.join_path(build_dir, libgama_name())

	return inst.get_zig().build_native(ZigBuildNativeOptions{
		file:      os.join_path(p.path, 'src', 'main.c')
		lib_path:  os.join_path(p.path, 'gama')
		build_dir: build_dir
		name:      conf.name
	})
}
