module vgama

import os
import term

pub fn (p Project) build_native(inst Installation) !string {
	conf := p.get_conf()!
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	// 1. Find the path to the pre-transpiled native.c runner.
	pretranspiled_c_path := os.join_path(inst.runners, 'native', 'native.c')

	if !os.exists(pretranspiled_c_path) {
		return error('pre-transpiled runner not found at ${pretranspiled_c_path}')
	}

	// 2. Copy the pre-transpiled C file to the build directory.
	copied_c_path := os.join_path(build_dir, 'native.c')
	os.cp(pretranspiled_c_path, copied_c_path) or {
		return error('Failed to copy pre-transpiled runner from ${pretranspiled_c_path} to ${copied_c_path}: ${err}')
	}

	// 3. Delegate to zig.v to compile both the student's C code and the runner's C code together.
	source_files := [
		os.join_path(p.path, 'src', 'main.c'),
		copied_c_path,
	]

	return inst.get_zig().build_native(ZigBuildNativeOptions{
		files:           source_files
		include_path:    inst.lib // Path to gama.h
		executable_path: os.join_path(build_dir, executable_extension(conf.name))
	})
}

pub fn (p Project) run_native_build(replace bool) ! {
	conf := p.get_conf()!
	output := os.join_path(p.path, 'build', 'native', executable_extension(conf.name))
	if !os.exists(output) {
		return error('Cannot run app, app was not built yet')
	}

	if replace {
		os.execvp(output, []) or {
			return error('Error launching the app build executable: ${err}')
		}
	} else {
		os.execute_opt(output) or { return error('Error running the app build executable: ${err}') }
	}
}
