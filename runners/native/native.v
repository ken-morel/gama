// This file is intended to be compiled as a shared library.
// Example: v -cc gcc -shared -o libgama.so runners/native/native.v
module main

import term

__global (
	abc int
)

@[export: 'gapi_init']
pub fn gapi_init() int {
	println(term.cyan('GAMA API (V Shared Library): gapi_init() called.'))
	return 0 // Indicate success
}

@[export: 'gapi_log']
pub fn gapi_log(message &char) {
	println(term.gray('GAMA LOG (from C): ${message.vstring()}'))
}

@[export: 'gapi_yield']
pub fn gapi_yield() f64 {
	return 0.01666 // ~16.6ms
}

@[export: 'gapi_quit']
pub fn gapi_quit() {
	println(term.cyan('GAMA API (V Shared Library): gapi_quit() called.'))
}
