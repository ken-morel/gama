module vgama

import gg
import time
import term
import sync

__global (
	gapi_ctx__           &gg.Context
	gapi_bg_color__      gg.Color
	gapi_c_can_draw__    &sync.Mutex
	gapi_v_can_present__ &sync.Mutex
	gapi_gama_runs__     bool
	gapi_title__         string
	gapi_width__         int
	gapi_height__        int
	gapi_side__          int
	gapi_queue__         []fn ()
	gapi_isfullscreen__  bool
	gapi_images__        map[u32]gg.Image
	gapi_image_count__   u32
)

fn frame(mut _ gg.Context) {
	// 1. Let the C thread know it can start drawing.
	gapi_c_can_draw__.unlock()
	// 2. Wait here until the C thread says it's done drawing for this frame.
	gapi_v_can_present__.lock()

	println('Calling draw functions ${gapi_queue__.len}')
	for func in gapi_queue__ {
		func()
	}
	println('called draw functions')
	gapi_queue__ = []
	gapi_ctx__.end()
}


fn run_gg_loop() {
	gapi_ctx__ = gg.new_context(
		width:        gapi_width__
		height:       gapi_height__
		window_title: gapi_title__
		frame_fn:     frame
		bg_color:     gapi_bg_color__
		resized_fn:   fn (e &gg.Event, data voidptr) {
			gapi_width__ = e.window_width
			gapi_height__ = e.window_height
			gapi_side__ = if gapi_width__ < gapi_height__ { gapi_width__ } else { gapi_height__ }
		}
			keydown_fn: fn(){}
			keyup_fn() {}
	)

	gapi_ctx__.run()
	gapi_gama_runs__ = false
	gapi_c_can_draw__.unlock() // Unlock C one last time to prevent deadlock on exit.
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init(width int, height int, title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))
	gapi_isfullscreen__ = false
	gapi_image_count__ = 1
	gapi_height__ = height
	gapi_width__ = width
	gapi_side__ = if gapi_width__ < gapi_height__ { gapi_width__ } else { gapi_height__ }
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

@[export: 'gapi_set_title']
@[unsafe]
fn gapi_set_title(title &char) {
	gapi_title__ = title.vstring()
	gg.set_window_title(gapi_title__)
}

@[export: 'gapi_log']
@[unsafe]
fn gapi_log(message &char) {
	println(term.gray('[log]: ${message.vstring()}'))
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield(dt &f64) i32 {
	mut static last_time := f64(0)

	// Signal V that we are done with the previous frame's drawing.
	gapi_v_can_present__.unlock()

	// Wait for V to prepare the new frame.
	gapi_c_can_draw__.lock()

	if !gapi_gama_runs__ {
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

	gapi_ctx__.begin()

	return 1
}

@[export: 'gapi_runs']
fn gapi_runs() i32 {
	return if gapi_gama_runs__ { i32(1) } else { i32(0) }
}

@[export: 'gapi_quit']
fn gapi_quit() {
	gapi_queue__ << fn () {
		gapi_ctx__.quit()
	}
	gapi_gama_runs__ = false
}

@[export: 'gapi_resize']
fn gapi_resize(w i32, h i32) {
	gapi_queue__ << fn [w, h] () {
		gapi_ctx__.resize(w, h)
	}
}

@[export: 'gapi_set_bg_color']
fn gapi_set_bg_color(r u8, g u8, b u8, a u8) {
	c := c_color(r, g, b, a)
	gapi_queue__ << fn [c] () {
		gapi_ctx__.set_bg_color(c)
	}
}

@[export: 'gapi_fullscreen']
fn gapi_fullscreen(fc i32) {
	gapi_queue__ << fn [fc] () {
		if fc == 1 && !gapi_isfullscreen__ {
			gg.toggle_fullscreen()
			gapi_isfullscreen__ = true
		} else if fc == 0 && gapi_isfullscreen__ {
			gg.toggle_fullscreen()
			gapi_isfullscreen__ = false
		}
	}
}
