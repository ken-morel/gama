import time
import term

type MayBeBool = bool | voidptr

@[unsafe]
fn v_gama_runs(val MayBeBool) bool {
	mut static value := false
	if val is bool {
		value = val
	}
	return value
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init() i32 {
	println(term.cyan('[vgama]: gapi_init() called'))
	v_gama_runs(true)
	println(term.ok_message('[vgama]: initialization succesful'))
	return 0
}

@[export: 'gapi_log']
@[unsafe]
fn gapi_log(message &char) {
	unsafe {
		println(term.gray('[log]: ${message.vstring()}'))
	}
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield() f64 {
	mut static last_time := f64(0)
	if last_time == 0 {
		last_time = get_time()
	}

	if !v_gama_runs(nil) {
		return 0.0
	}
	current_time := get_time()
	dt_seconds := current_time - last_time
	last_time = current_time
	return dt_seconds
}

@[export: 'gapi_runs']
@[unsafe]
fn gapi_runs() i32 {
	return if v_gama_runs(nil) { i32(1) } else { i32(0) }
}

@[export: 'gapi_quit']
@[unsafe]
fn gapi_quit() {
	println(term.cyan('[vgama]: gapi_quit() called'))
	v_gama_runs(false)
}
