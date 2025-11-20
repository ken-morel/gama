module vgama

import os
import term

pub fn (p Project) build_native(inst Installation) !string {
	conf := p.get_conf()!
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	// TODO: This path should eventually be '${$os}_${$arch}'
	runner_path := os.join_path(inst.runners, 'native')
	if !os.exists(runner_path) {
		return error('no pre-compiled native runner found at ${runner_path}')
	}
	os.cp_all(runner_path, build_dir, false) or {
		return error('Failed to copy runner from ${runner_path} to ${build_dir}: ${err}')
	}
	return inst.get_zig().build_native(ZigBuildNativeOptions{
		file:      os.join_path(p.path, 'src', 'main.c')
		lib_path:  os.join_path(p.path, 'gama')
		build_dir: build_dir
		name:      conf.name
	})
}

pub fn (p Project) run_native_build() ! {
	conf := p.get_conf()!
	output := os.join_path(p.path, 'build', 'native', executable_extension(conf.name))
	if !os.exists(output) {
		return error('Cannot run app, app was not built yet')
	}

	os.execute_opt(output) or { return error('Error running the app build executable: ${err}') }
}
