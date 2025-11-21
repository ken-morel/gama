module main

import gg
import time
import term
import sync

__global (
	gapi_g__             &gg.Context
	gapi_bg_color__      gg.Color
	gapi_c_can_draw__    &sync.Mutex
	gapi_v_can_present__ &sync.Mutex
	gapi_gama_runs__     bool
	gapi_title__         string
	gapi_width__         int
	gapi_height__        int
)

// The frame function for the gg loop (runs in V thread).
fn frame(mut ctx gg.Context) {
	// 1. Let the C thread know it can start drawing.
	gapi_c_can_draw__.unlock()
	// 2. Wait here until the C thread says it's done drawing for this frame.
	gapi_v_can_present__.lock()
	// 3. Present the frame (drawing happens on the C side).
	ctx.end()
}

// This function runs in the spawned thread.
fn run_gg_loop() {
	gapi_g__ = gg.new_context(
		width:        gapi_width__
		height:       gapi_height__
		window_title: gapi_title__
		frame_fn:     frame
		bg_color:     gapi_bg_color__
	)
	gapi_g__.run() // This blocks until the window is closed.
	gapi_gama_runs__ = false // Signal the main C loop to exit.
	gapi_c_can_draw__.unlock() // Unlock C one last time to prevent deadlock on exit.
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
fn gapi_init(width int, height int, title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))
	gapi_height__ = height
	gapi_width__ = width
	gapi_title__ = title.vstring()

	gapi_bg_color__ = gg.rgb(100, 100, 100)
	gapi_c_can_draw__ = &sync.Mutex{}
	gapi_c_can_draw__.lock()
	gapi_v_can_present__ = &sync.Mutex{}
	gapi_v_can_present__.lock()

	// Spawn the graphics thread.
	spawn run_gg_loop()
	// Wait for the graphics thread to signal that it's ready (after the first frame).
	gapi_c_can_draw__.lock()
	gapi_gama_runs__ = true

	println(term.ok_message('[vgama]: initialization successful'))
	return 0
}

@[export: 'gapi_log']
fn gapi_log(message &char) {
	println(term.gray('[log]: ${message.vstring()}'))
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield(dt &f64) i32 {
	println(term.cyan('yielding'))
	mut static last_time := f64(0)

	// Signal V that we are done with the previous frame's drawing.
	gapi_v_can_present__.unlock()

	// Wait for V to prepare the new frame.
	gapi_c_can_draw__.lock()

	// If the V thread has exited, `gapi_gama_runs__` will be false.
	if !gapi_gama_runs__ {
		return 0
	}

	// First time through, last_time will be 0.
	if last_time == 0 {
		last_time = get_time()
	}

	current_time := get_time()
	if dt != nil {
		*dt = current_time - last_time
	}
	last_time = current_time

	// Now that we have the lock, C-side can draw.
	gapi_g__.begin()

	return 1
}

@[export: 'gapi_runs']
fn gapi_runs() i32 {
	return if gapi_gama_runs__ { i32(1) } else { i32(0) }
}

@[export: 'gapi_quit']
fn gapi_quit() {
	gapi_gama_runs__ = false
}
