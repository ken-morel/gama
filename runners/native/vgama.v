module main

// import gg
import time
import term
// import sync

__global (
	// gapi_g__             &gg.Context
	// gapi_bg_color__      gg.Color
	// gapi_c_can_draw__    &sync.Mutex
	// gapi_v_can_present__ &sync.Mutex
	gapi_gama_runs__ bool
)

// The frame function for the gg loop (runs in V thread).
// fn frame(mut ctx gg.Context) {
// 	// 1. Let the C thread know it can start drawing.
// 	gapi_c_can_draw__.unlock()
// 	// 2. Wait here until the C thread says it's done drawing for this frame.
// 	gapi_v_can_present__.lock()
// 	// 3. Present the frame (drawing happens on the C side).
// 	ctx.end()
// }

// This function runs in the spawned thread.
fn run_gg_loop() {
	// gapi_g__.run() // This blocks until the window is closed.
	gapi_gama_runs__ = false // Signal the main C loop to exit.
	// gapi_c_can_draw__.unlock() // Unlock C one last time to prevent deadlock on exit.
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
fn gapi_init(width int, height int, title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))

	// gapi_bg_color__ = gg.rgb(100, 100, 100)
	// gapi_c_can_draw__ = &sync.Mutex{}
	// gapi_v_can_present__ = &sync.Mutex{}

	// gapi_g__ = gg.new_context(
	// 	width:        width
	// 	height:       height
	// 	window_title: title.vstring()
	// 	frame_fn:     frame
	// 	bg_color:     gapi_bg_color__
	// )

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
	mut static last_time := f64(0)

	if last_time == 0 {
		// First call: spawn the V thread, then wait for it to be ready.
		// spawn run_gg_loop()
		gapi_gama_runs__ = true
	} else {
		// Subsequent calls: Signal V that we are done with the previous frame.
		// gapi_v_can_present__.unlock()
	}

	// For all calls (including the first), wait for V to be ready for the new frame.
	// gapi_c_can_draw__.lock()

	// If the V thread has exited, `gapi_gama_runs__` will be false.
	if !gapi_gama_runs__ {
		return 0
	}

	current_time := get_time()
	if dt != nil {
		*dt = current_time - last_time
	}
	last_time = current_time

	// Now that we have the lock, C-side can draw. Let's clear the screen.
	// gapi_g__.begin()

	return 1
}
