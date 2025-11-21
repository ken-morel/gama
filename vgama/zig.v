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
	files           []string @[required] // C source files
	include_path    string   @[required] // for -I
	executable_path string   @[required] // full path for the output exe
}

fn (z ZigCC) build_native(opts ZigBuildNativeOptions) !string {
	// Compile and link multiple C source files together.
	source_files_str := opts.files.join(' ')
	mut linker_flags := '-lpthread'
	mut defines := ' -DSOKOL_IMPL -DSOKOL_NO_ENTRY'

	$if linux {
		linker_flags += ' -lX11 -lGL -lglfw'
		defines += ' -DSOKOL_GLCORE'
	} $else $if windows {
		linker_flags += ' -lgdi32 -luser32'
		defines += ' -DSOKOL_D3D11'
	} $else $if macos {
		defines += ' -DSOKOL_METAL'
		linker_flags += ' -framework Cocoa -framework OpenGL'
	}

	cmd := '${z.exepath} cc -o ${opts.executable_path} ${source_files_str} -I${opts.include_path} ${defines} ${linker_flags}'
	res := os.execute(cmd)
	return if res.exit_code != 0 {
		error('Failed to compile and link app: ${res.output}')
	} else {
		opts.executable_path
	}
}
