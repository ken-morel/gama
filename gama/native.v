module gama

import os
import term

pub fn (p Project) get_src_c_files() []string {
	return (os.glob(os.join_path(p.path, 'src', '**.c')) or { [] }).filter(it.ends_with('.c'))
}

pub fn (p Project) copy_build_native_artifacts(inst Installation, reset bool) ! {
	build_dir := p.build_path('native')
	runner_path := os.join_path(inst.runners, 'native')
	os.mkdir_all(build_dir) or {}
	src := os.join_path(runner_path, libvgama_name())
	dest := os.join_path(build_dir, libvgama_name())
	if !os.exists(dest) || reset {
		os.cp(src, dest) or { return error('Failed to copy libvgama: ${err}') }
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

fn resolve_compiler(inst Installation, name string) !string {
	return if name == '' {
		inst.zcc()
	} else if name == '.tcc' {
		inst.tcc_exe()
	} else if name == '.zcc' {
		inst.zcc()
	} else {
		name
	}
}

pub fn (p Project) build_native(inst Installation, use_cc string) !string {
	conf := p.get_conf()!
	p.bake(inst, false) or { println(term.warn_message('Baking failed: ${err}')) }
	compiler := resolve_compiler(inst, use_cc)!

	// Prepare common build parameters
	build_dir := p.build_path('native')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }
	source_files := p.get_src_c_files()
	if source_files.len == 0 {
		return error('No c source files in src directory')
	}

	executable_path := os.join_path(build_dir, executable_extension(conf.name))

	include_path := os.join_path(p.path, 'include')
	gen_path := p.build_path('gen')
	cmd := "${compiler} -o ${executable_path} ${source_files.join(' ')} -I${include_path} -I${gen_path} -L${build_dir} -Wl,-rpath,'\$ORIGIN' -DGM_NATIVE -lvgama -lm -v"
	println('Executing: ${cmd}')
	res := os.execute(cmd)
	if res.exit_code != 0 {
		return error('Failed to build app: ${res.output}')
	} else {
		println(res.output)
	}

	return executable_path
}
