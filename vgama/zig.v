module vgama

import os

struct ZigCC {
	exepath string
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
	// mut linker_flags := ""

	// $if linux {
	// 	// linker_flags += ' -lX11 -lGL -lglfw' // Common for gg on Linux
	// } $else $if windows {
	// 	// linker_flags += ' -lgdi32 -luser32' // Common for gg on Windows
	// }
	// TODO: Add macOS flags when needed: linker_flags += ' -framework Cocoa -framework OpenGL'

	cmd := 'tcc -o ${opts.executable_path} ${source_files_str} -I${opts.include_path} -L${opts.lib_path} -Wl,-rpath,\'\$ORIGIN\' -lvgama -lm -v'
	println('Building with: ${cmd}')
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.output}')
	} else {
		opts.executable_path
	}
}
