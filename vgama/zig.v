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
	if opts.files.len == 0 {
		return error('No c source file passed to builder')
	}
	source_files_str := opts.files.join(' ')
	mut linker_flags := '-lvgama -Wl,-rpath=.'

	$if linux {
		// linker_flags += ' -lX11 -lGL -lglfw' // Common for gg on Linux
	} $else $if windows {
		// linker_flags += ' -lgdi32 -luser32' // Common for gg on Windows
	}
	// TODO: Add macOS flags when needed: linker_flags += ' -framework Cocoa -framework OpenGL'

	cmd := '${z.exepath} cc -o ${opts.executable_path} ${source_files_str} -I${opts.include_path} -L${opts.lib_path} ${linker_flags} -v'
	// cmd := '${z.exepath} cc -o /home/engon/gama/ama/build/native/ama /home/engon/gama/ama/src/main.c -I/home/engon/gama/ama/gama -L/home/engon/gama/ama/build/native -lvgama -Wl,-rpath=.'
	println('Building with: ${cmd}')
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.output}')
	} else {
		opts.executable_path
	}
}
