module vgama

import os

struct ZigCC {
	exepath string
}

fn (z ZigCC) build_shared(file string, include string, name string, out string) !string {
	ext := get_shared_library_extension()
	output := os.join_path(out, '${name}.gama.${ext}')
	compile_cmd := '${z.exepath} cc -shared -o ${output} ${file} -I${include}'
	res := os.execute(compile_cmd)

	return if res.exit_code != 0 {
		error('Failed to compile code: ${res.output}')
	} else {
		output
	}
}

struct ZigBuildNativeOptions {
	file      string @[required]
	lib_path  string @[required]
	build_dir string @[required]
	name      string @[required]
}

fn (z ZigCC) build_native(opts ZigBuildNativeOptions) !string {
	output = os.join_path(opts.build_dir, executable_extention(opts.name))
	cmd := '${z.exepath} cc -o ${output} ${opts.file} -I${opts.lib_path} -L${opts.build_dir} -lgama -Wl,-rpath=.'
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.ouptut}')
	} else {
		output
	}
}
