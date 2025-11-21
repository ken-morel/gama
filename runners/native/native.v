module vgama

import gg
import time
import term
import sync

__global (
	g         &gg.Context
	bg_color  gg.Color
	bottle    &sync.Mutex
	gama_runs bool
)

fn run_gg_loop() {
	g.run()
	gama_runs = false
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init(width int, height int, title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))

	bg_color = gg.rgb(100, 100, 100)
	bottle = &sync.Mutex{}

	g = gg.new_context(
		width:        width
		height:       height
		window_title: title.vstring()
		frame_fn:     frame
		bg_color:     bg_color
	)

	println(term.ok_message('[vgama]: initialization successful'))
	return 0
}

@[export: 'gapi_log']
@[unsafe]
fn gapi_log(message &char) {
	println(term.gray('[log]: ${message.vstring()}'))
}

fn frame(mut ctx gg.Context) {
	bottle.unlock() // let c start drawing
	time.sleep(10)
	bottle.lock() // wait for c, then block it again
	ctx.end()
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield(dt &f64) i32 {
	mut static last_time := f64(0)

	if last_time == 0 {
		last_time = get_time()
		spawn run_gg_loop()
		bottle.lock() // v // it is not locked yet, tell c to wait
		gama_runs = true
	} else {
		bottle.unlock() // let v run, tell them we finished drawing
		time.sleep(10)
	}

	current_time := get_time()
	unsafe {
		if dt != nil {
			*dt = current_time - last_time
		}
	}
	last_time = current_time

	bottle.lock() // c // wait for v to prepare
	return gapi_runs()
}

@[export: 'gapi_runs']
@[unsafe]
fn gapi_runs() i32 {
	return if gama_runs { i32(1) } else { i32(0) }
}

@[export: 'gapi_quit']
@[unsafe]
fn gapi_quit() {
	println(term.cyan('[vgama]: gapi_quit() called'))
	gama_runs = false
}
