module vgama

import os

pub fn (p Project) build_native(inst Installation) !string {
	conf := p.get_conf()!
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	// 1. Copy the pre-transpiled C file and STB implementation to the build directory.
	lib_basename := libvgama_name()
	lib_path := os.join_path(inst.runners, 'native', lib_basename)

	if !os.exists(lib_path) {
		return error('libvgama not found at ${lib_path}')
	}
	os.cp(lib_path, os.join_path(build_dir, lib_basename)) or {
		return error('Failed to copy libvgama: ${err}')
	}

	source_files := p.get_src_c_files()!

	return inst.get_zig().build_native(ZigBuildNativeOptions{
		files:           source_files
		include_path:    inst.lib
		lib_path:        build_dir
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

pub fn (p Project) get_src_c_files() ![]string {
	mut files := []string{}
	src := os.join_path(p.path, 'src')
	os.walk(src, fn [mut files] (file string) {
		files << file
	})
	return files
}
