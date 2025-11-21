module main

import gg
import time
import term
import sync

__global (
	ctx           &gg.Context
	bg_color      gg.Color
	c_can_draw    &sync.Mutex
	v_can_present &sync.Mutex
	gama_runs     bool
	title         string
	width         int
	height        int
)

fn frame(mut _ gg.Context) {
	// 1. Let the C thread know it can start drawing.
	c_can_draw.unlock()
	// 2. Wait here until the C thread says it's done drawing for this frame.
	v_can_present.lock()
	ctx.end()
}

fn run_gg_loop() {
	ctx = gg.new_context(
		width:        width
		height:       height
		window_title: title
		frame_fn:     frame
		bg_color:     bg_color
	)
	ctx.run()
	gama_runs = false
	c_can_draw.unlock() // Unlock C one last time to prevent deadlock on exit.
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init(width int, height int, c_title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))
	height = height
	width = width
	title = c_title.vstring()

	bg_color = gg.rgb(100, 100, 100)
	c_can_draw = &sync.Mutex{}
	c_can_draw.lock()
	v_can_present = &sync.Mutex{}
	v_can_present.lock()

	// Spawn the graphics thread.
	spawn run_gg_loop()
	// Wait for the graphics thread to signal that it's ready (after the first frame).
	c_can_draw.lock()
	gama_runs = true

	println(term.ok_message('[vgama]: initialization successful'))
	return 0
}

@[export: 'gapi_log']
@[unsafe]
fn gapi_log(message &char) {
	println(term.gray('[log]: ${message.vstring()}'))
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield(dt &f64) i32 {
	println(term.cyan('yielding'))
	mut static last_time := f64(0)

	// Signal V that we are done with the previous frame's drawing.
	v_can_present.unlock()

	// Wait for V to prepare the new frame.
	c_can_draw.lock()

	if !gama_runs {
		return 0
	}

	if last_time == 0 {
		last_time = get_time()
	}

	current_time := get_time()
	if dt != nil {
		*dt = current_time - last_time
	}
	last_time = current_time

	ctx.begin()

	return 1
}

@[export: 'gapi_runs']
fn gapi_runs() i32 {
	return if gama_runs { i32(1) } else { i32(0) }
}

@[export: 'gapi_quit']
fn gapi_quit() {
	gama_runs = false
}
