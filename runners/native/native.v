module main

import time
import term

__global (
	gama_runs       bool
	last_yield_time f64
)

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
fn gapi_init() i32 {
	println(term.cyan('[vgama]: gapi_init() called'))
	gama_runs = true
	last_yield_time = get_time()
	println(term.ok_message('[vgama]: initialization succesful'))
	return 0
}

@[export: 'gapi_log']
fn gapi_log(message &char) {
	unsafe {
		println(term.gray('[log]: ${message.vstring()}'))
	}
}

@[export: 'gapi_yield']
fn gapi_yield() f64 {
	if !gama_runs {
		return 0.0
	}
	current_time := get_time()
	dt_seconds := current_time - last_yield_time
	last_yield_time = current_time
	return dt_seconds
}

@[export: 'gapi_runs']
fn gapi_runs() i32 {
	return if gama_runs { 1 } else { 0 }
}

@[export: 'gapi_quit']
fn gapi_quit() {
	println(term.cyan('[vgama]: gapi_quit() called'))
	gama_runs = false
}
