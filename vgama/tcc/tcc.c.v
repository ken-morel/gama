module tcc

// import os

// #flag -L../tcc
// #flag -ltcc
// #flag -I../tcc
// #include "libtcc.h"

// TCC output types, mirrored from libtcc.h for readability
const output_memory = 0
const output_exe = 1
const output_dll = 2
const output_obj = 3

// // Redeclare the C struct TCCState
// struct C.TCCState {}

// // Redeclare the necessary C functions from libtcc.h
// fn C.tcc_new() &C.TCCState
// fn C.tcc_delete(&C.TCCState)
// fn C.tcc_set_output_type(&C.TCCState, int) int
// fn C.tcc_add_include_path(&C.TCCState, &char) int
// fn C.tcc_add_library_path(&C.TCCState, &char) int
// fn C.tcc_add_library(&C.TCCState, &char) int
// fn C.tcc_add_file(&C.TCCState, &char) int
// fn C.tcc_output_file(&C.TCCState, &char) int
// fn C.tcc_set_error_func(&C.TCCState, voidptr, fn (voidptr, &char))
// fn C.tcc_set_lib_path(&C.TCCState, &char)

// V-native struct for build options
pub struct BuildOptions {
pub:
	source_files      []string
	output_executable string
	include_paths     []string
	library_paths     []string
	libraries         []string
	tcc_path          string
}

// Error callback function to be passed to TCC
fn tcc_error_callback(opaque voidptr, msg &char) {
	eprintln('TCC Error: ${unsafe { cstring_to_vstring(msg) }}')
}

// Public V function to build an executable
pub fn build_exe(opts BuildOptions) ! {
	println('The embedded tcc compiler was disabled')
	// state := C.tcc_new()
	// if state == unsafe { nil } {
	// 	return error('Failed to create TCC state')
	// }
	// defer {
	// 	C.tcc_delete(state)
	// }

	// // Set TCC's internal library path (where it finds crt1.o, libtcc1.a, etc.)
	// C.tcc_set_lib_path(state, &char(os.join_path(opts.tcc_path, 'lib', 'linux').str))

	// // Set error function
	// C.tcc_set_error_func(state, unsafe { nil }, tcc_error_callback)

	// // Set output type to executable
	// if C.tcc_set_output_type(state, output_exe) != 0 {
	// 	return error('Failed to set output type to EXE')
	// }

	// // Add include paths
	// for path in opts.include_paths {
	// 	if C.tcc_add_include_path(state, &char(path.str)) != 0 {
	// 		return error('Failed to add include path: ${path}')
	// 	}
	// }
	// C.tcc_add_include_path(state, c'/usr/include')
	// C.tcc_add_include_path(state, c'/usr/include/x86_64-linux-gnu')
	// C.tcc_add_include_path(state, &char(os.join_path(opts.tcc_path, 'include', 'linux').str))

	// // Add system library path for things like libm.so

	// if C.tcc_add_library_path(state, c'/usr/lib/x86_64-linux-gnu/') != 0 {
	// 	return error('Failed to add system library path /usr/lib')
	// }
	// if C.tcc_add_library_path(state, c'/usr/lib/') != 0 {
	// 	return error('Failed to add system library path /usr/lib')
	// }
	// // Add project-specific library paths
	// for path in opts.library_paths {
	// 	if C.tcc_add_library_path(state, &char(path.str)) != 0 {
	// 		return error('Failed to add library path: ${path}')
	// 	}
	// }

	// // Add libraries
	// for lib in opts.libraries {
	// 	if C.tcc_add_library(state, &char(lib.str)) != 0 {
	// 		return error('Failed to add library: ${lib}')
	// 	}
	// }

	// // Add source files
	// for file in opts.source_files {
	// 	if C.tcc_add_file(state, &char(file.str)) == -1 {
	// 		return error('Failed to add source file: ${file}')
	// 	}
	// }

	// // Compile and output the file
	// if C.tcc_output_file(state, &char(opts.output_executable.str)) == -1 {
	// 	return error('Failed to output executable file')
	// }
}
