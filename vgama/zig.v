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
	files           []string @[required]
	include_path    string   @[required]
	executable_path string   @[required]
	lib_path        string   @[required]
}

fn (z ZigCC) build_native(opts ZigBuildNativeOptions) !string {
	source_files_str := opts.files.join(' ')

	cmd := '${z.exepath} cc -o ${opts.executable_path} ${source_files_str} -I${opts.include_path}'
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.output}')
	} else {
		opts.executable_path
	}
}
