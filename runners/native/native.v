import gg
import gx
import time
import term
import sync

__global (
	g             &gg.Context
	bg_color      gx.Color
	c_can_draw    sync.Mutex
	v_can_present sync.Mutex
	gama_runs     bool
)

// The frame function for the gg loop (runs in V thread).
fn frame(mut ctx gg.Context) {
	// 1. Clear the screen for the new frame.
	ctx.clear(bg_color)
	// 2. Let the C thread know it can start drawing.
	c_can_draw.unlock()
	// 3. Wait here until the C thread says it's done drawing for this frame.
	v_can_present.lock()
	// 4. Present the frame.
	ctx.end()
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_init']
@[unsafe]
fn gapi_init(width int, height int, title &char) i32 {
	println(term.cyan('[vgama]: gapi_init() called'))

	bg_color = gx.rgb(100, 100, 100)
	c_can_draw = sync.new_mutex()
	v_can_present = sync.new_mutex()
	// Deliberately NOT locking here to prevent deadlock on first frame.

	g = gg.new_context(
		width:        width
		height:       height
		window_title: title.vstring()
		frame_fn:     frame
	)

	spawn g.run()

	gama_runs = true
	println(term.ok_message('[vgama]: initialization successful'))
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
fn gapi_yield(dt &f64) i32 {
	mut static last_time := f64(0)

	// This is the "ping-pong" synchronization logic.
	if last_time == 0 {
		// First call. Don't do any locking/unlocking.
		// Just bootstrap the timing and let the loop run once.
		last_time = get_time()
	} else {
		// Subsequent calls.
		// 1. Signal to V that C-side drawing for the *previous* frame is complete.
		v_can_present.unlock()
		// 2. Wait here until V has cleared the *new* frame and is ready for drawing commands.
		c_can_draw.lock()
	}

	if !g.running {
		gama_runs = false
	}

	current_time := get_time()
	unsafe {
		if dt != nil {
			*dt = current_time - last_time
		}
	}
	last_time = current_time

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
