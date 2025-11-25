module vgama

import os

pub fn (p Project) copy_build_native_artifacts(inst Installation) ! {
	build_dir := p.build_path('native')
	runner_path := os.join_path(inst.runners, 'native')
	os.mkdir_all(build_dir) or {}
	os.cp(os.join_path(runner_path, libvgama_name()), os.join_path(build_dir, libvgama_name())) or {
		return error('Failed to copy libvgama: ${err}')
	}
}

pub fn (p Project) build_native(inst Installation) !string {
	conf := p.get_conf()!
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }

	runner_path := os.join_path(inst.runners, 'native')

	if !os.exists(runner_path) {
		return error('Gama native runner not found at ${runner_path}, native build not supported')
	}

	p.copy_build_native_artifacts(inst)!
	source_files := p.get_src_c_files()!

	if source_files.len == 0 {
		return error('No c source files in src directory')
	}
	include_path := os.join_path(p.path, 'include')
	executable_path := os.join_path(build_dir, executable_extension(conf.name))
	cmd := "${conf.gama.compiler} -o ${executable_path} ${source_files.join(' ')} -I${include_path} -L${build_dir} -Wl,-rpath,'\$ORIGIN' -lvgama -lm -v"
	println('Building with: ${cmd}')
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.output}')
	} else {
		executable_path
	}
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
	return os.glob(os.join_path(p.path, 'src', '**.c'))
}
