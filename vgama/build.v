module vgama

import os
import tcc

pub fn (p Project) copy_build_native_artifacts(inst Installation) ! {
	build_dir := p.build_path('native')
	runner_path := os.join_path(inst.runners, 'native')
	os.mkdir_all(build_dir) or {}
	os.cp(os.join_path(runner_path, libvgama_name()), os.join_path(build_dir, libvgama_name())) or {
		return error('Failed to copy libvgama: ${err}')
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

pub fn (p Project) build_native(inst Installation, force_tcc bool) !string {
	conf := p.get_conf()!
	use_tcc := false // force_tcc || conf.gama.compiler == ''
	if conf.gama.compiler == '' {
		return error('No compiler configured')
	}

	// Prepare common build parameters
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }
	p.copy_build_native_artifacts(inst)!
	source_files := p.get_src_c_files()!
	if source_files.len == 0 {
		return error('No c source files in src directory')
	}
	inst.copy_gama(os.join_path(p.path, 'include'), false) or {}

	executable_path := os.join_path(build_dir, executable_extension(conf.name))

	if use_tcc {
		println('Building with embedded TCC...')
		tcc_opts := tcc.BuildOptions{
			source_files:      source_files
			output_executable: executable_path
			include_paths:     [os.join_path(p.path, 'include')]
			library_paths:     [build_dir]
			libraries:         ['vgama']
			tcc_path:          os.join_path(inst.runners, 'tcc')
		}
		tcc.build_exe(tcc_opts)!
	} else {
		println('Building with external compiler: ${conf.gama.compiler}')
		include_path := os.join_path(p.path, 'include')
		cmd := "${conf.gama.compiler} -o ${executable_path} ${source_files.join(' ')} -I${include_path} -L${build_dir} -Wl,-rpath,'\$ORIGIN' -lvgama -lm -v"
		println('Executing: ${cmd}')
		res := os.execute(cmd)
		if res.exit_code != 0 {
			return error('Failed to compile and link app: ${res.output}')
		}
	}

	return executable_path
}
