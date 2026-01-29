module vgama

import gg
import term
import os
import rand

// Define miniaudio C types and declare functions
// Use a more compatible graphics backend for Sokol on Windows.
// #flag -Wl,-Bstatic
// #flag -D_SGL_DEFAULT_MAX_COMMANDS=65536
// #flag -D_SGL_DEFAULT_MAX_VERTICES=4194304

// These flags are only for Windows to reduce external .dll dependencies.
#flag -static-libgcc
#flag -static-libstdc++

$if windows {
	// Force Sokol to use its legacy OpenGL context for maximum compatibility on Windows,
	// especially in VMs with basic graphics drivers.
	#flag -DSOKOL_GL_FORCE_LEGACY
}

type GapiTask = fn ()

const draw_instruction_count = 10000

enum GapiWinsizeMode {
	auto
	fixed
}

__global (
	gapi_ctx__          &gg.Context
	gapi_bg_color__     gg.Color
	gapi_gama_runs__    bool
	gapi_title__        string
	gapi_width__        int
	gapi_height__       int
	gapi_queue__        chan []GapiTask
	gapi_buff__         []GapiTask
	gapi_end_frame__    chan bool
	gapi_images__       map[i32]gg.Image
	gapi_image_count__  u16
	// viewport
	gapi_game_w__       int
	gapi_game_h__       int
	gapi_offset_x__     int
	gapi_offset_y__     int
	gapi_queue_wait__   &sync.Mutex
	// event
	gapi_pressed_keys__ []string
	gapi_mouse_x__      i32
	gapi_mouse_y__      i32
	gapi_mouse_down__   bool
	// files
	gapi_dir__          string
	gapi_winsize_mode__ GapiWinsizeMode
)

fn update_dimensions() {
	size := gapi_ctx__.window_size()
	gapi_width__ = size.width
	gapi_height__ = size.height
}

fn update_virtual_dimensions() {
	if gapi_width__ < gapi_height__ {
		gapi_game_w__ = gapi_width__
		gapi_game_h__ = gapi_width__
		gapi_offset_x__ = 0
		gapi_offset_y__ = (gapi_height__ - gapi_width__) / 2
	} else {
		gapi_game_w__ = gapi_height__
		gapi_game_h__ = gapi_height__
		gapi_offset_x__ = (gapi_width__ - gapi_height__) / 2
		gapi_offset_y__ = 0
	}
}

fn frame(mut _ gg.Context) {
	gapi_ctx__.begin()
	gapi_ctx__.end(how: .clear)

	gapi_ctx__.begin()
	for {
		select {
			funcs := <-gapi_queue__ {
				gapi_ctx__.begin()
				for func in funcs {
					func()
				}
				gapi_ctx__.end(how: .passthru)
			}
			_ := <-gapi_end_frame__ {
				break
			}
		}
	}
	gapi_ctx__.end(how: .passthru)
}

fn queue_buff() {
	if gapi_buff__.len > 0 {
		gapi_queue__ <- gapi_buff__
		gapi_buff__ = []GapiTask{cap: draw_instruction_count}
	}
}

fn queue_fn(func GapiTask) {
	gapi_buff__ << func
	if gapi_buff__.len >= draw_instruction_count {
		queue_buff()
	}
}

@[export: 'gapi_sync']
fn gapi_sync() {
	queue_fn(fn () {
		gapi_queue_wait__.unlock()
	})
	queue_buff()
	gapi_queue_wait__.lock() // wait all preceding events are processed
}

@[export: 'gapi_yield']
@[unsafe]
fn gapi_yield(dt &f64) i32 {
	if !gapi_gama_runs__ {
		return 0
	}
	gapi_sync() // wait it processes other events before sending stop
	gapi_end_frame__ <- true or { return 0 } // close the current frame

	gapi_pressed_keys__ = []

	// for the frame to request for closing
	//
	// subsequent pushes to the queue will block
	mut static last_time := f64(0)

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

	return if gapi_gama_runs__ { 1 } else { 0 }
}

fn run_gg_loop() {
	gapi_ctx__ = gg.new_context(
		fullscreen:   gapi_winsize_mode__ == .auto
		width:        gapi_width__
		height:       gapi_height__
		window_title: gapi_title__
		frame_fn:     frame
		bg_color:     gapi_bg_color__
		fail_fn:      fn (msg string, _ voidptr) {
			println(term.fail_message(msg))
		}
		resized_fn:   fn (e &gg.Event, _ voidptr) {
			gapi_width__ = e.window_width
			gapi_height__ = e.window_height
			update_virtual_dimensions()
		}
		keydown_fn:   fn (code gg.KeyCode, _ gg.Modifier, _ voidptr) {
			if key := keys[code] {
				gapi_pressed_keys__ << key
			}
		}
		move_fn:      fn (x f32, y f32, _ voidptr) {
			gapi_mouse_x__ = i32(x)
			gapi_mouse_y__ = i32(y)
		}
		click_fn:     fn (x f32, y f32, _ gg.MouseButton, _ voidptr) {
			gapi_mouse_x__ = i32(x)
			gapi_mouse_y__ = i32(y)
			gapi_mouse_down__ = true
		}
		unclick_fn:   fn (x f32, y f32, _ gg.MouseButton, _ voidptr) {
			gapi_mouse_x__ = i32(x)
			gapi_mouse_y__ = i32(y)
			gapi_mouse_down__ = false
		}
		cleanup_fn:   fn (data voidptr) {
			gapi_gama_runs__ = false
			// Uninitialize miniaudio engine
			audio_deinit()
		}
		init_fn:      fn (data voidptr) {
			update_dimensions()
			update_virtual_dimensions()
		}
	)

	println(term.cyan('[vgama] Starting app'))
	gapi_ctx__.run()
	println(term.cyan('[vgama] App quited'))
	gapi_gama_runs__ = false
	gapi_queue__.close() // cancel remaining draw operaions
	gapi_end_frame__.close()
	println(term.cyan('[vgama] bye'))
	gapi_queue_wait__.unlock()

	gapi_end_frame__ <- true or {} // close the current frame
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init(width int, height int, title &char) i32 {
	if width * height == 0 {
		gapi_winsize_mode__ = .auto
		size := gg.screen_size()
		gapi_width__ = size.width
		gapi_height__ = size.height
	} else {
		gapi_winsize_mode__ = .fixed
		gapi_height__ = height
		gapi_width__ = width
	}
	println(term.cyan('[vgama]: gapi_init() called'))
	gapi_image_count__ = 1
	gapi_title__ = title.vstring()

	gapi_dir__ = os.join_path(os.temp_dir(), rand.uuid_v7())
	os.mkdir_all(gapi_dir__) or { term.warn_message('Could not create app temporary directory') }
	println(term.cyan('\n[vgama] Using temporary directory: ${gapi_dir__}'))

	gapi_bg_color__ = gg.rgb(100, 100, 100)

	gapi_queue__ = chan []GapiTask{cap: 1000}
	gapi_buff__ = []GapiTask{cap: draw_instruction_count}
	gapi_end_frame__ = chan bool{cap: 0}
	gapi_queue_wait__ = &sync.Mutex{}
	gapi_queue_wait__.lock()

	audio_init()

	// Spawn the graphics thread.
	spawn run_gg_loop()
	// Wait for the graphics thread to signal that it's ready (after the first frame).
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

@[export: 'gapi_quit']
fn gapi_quit() {
	queue_fn(fn () {
		gapi_ctx__.quit()
	})
}

@[export: 'gapi_resize']
fn gapi_resize(w i32, h i32) {
	queue_fn(fn [w, h] () {
		gapi_ctx__.resize(w, h)
	})
}

@[export: 'gapi_get_size']
@[unsafe]
fn gapi_get_size(w &u32, h &u32) {
	size := gapi_ctx__.window_size()
	*w = u32(size.width)
	*h = u32(size.height)
}

@[export: 'gapi_set_background_color']
fn gapi_set_bg_color(col GmColor) {
	c := col.to_gg()
	queue_fn(fn [c] () {
		gapi_ctx__.set_bg_color(c)
	})
}

@[export: 'gapi_fullscreen']
fn gapi_fullscreen(fc i32) {
	queue_fn(fn [fc] () {
		if fc == 1 && !gg.is_fullscreen() {
			gg.toggle_fullscreen()
		} else if fc == 0 && gg.is_fullscreen() {
			gg.toggle_fullscreen()
		}
	})
}
