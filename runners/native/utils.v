module vgama

import gg
import time
import term

// Correctly converts a normalized coordinate (-1.0 to 1.0) to a screen pixel coordinate,
// taking the centered "virtual square" game area into account.
fn c_redimension_coord(x f64, y f64) (f32, f32) {
	// Map -1 to 1 range to the 0 to 1 range
	norm_x := (x + 1.0) * 0.5
	norm_y := (1.0 - y) * 0.5 // Invert Y-axis for screen coordinates

	// Scale to the virtual game area and add the offset to center it
	screen_x := f32(norm_x * gapi_game_w__ + gapi_offset_x__)
	screen_y := f32(norm_y * gapi_game_h__ + gapi_offset_y__)
	
	return screen_x, screen_y
}

// Scales a single normalized scalar value (like a radius or thickness) to the virtual game area.
// It maps the normalized value (which is typically -1 to 1 for coords, but 0 to 2 for dimensions)
// to pixels based on the smallest dimension of the virtual game area.
fn c_redimension_one(v f64) f32 {
	// gapi_game_w__ and gapi_game_h__ are the same (side of the square), so we can use either
	// The 0.5 factor is because a normalized width/height of 1.0 means full height, so we scale it.
	return f32(v * gapi_game_w__ * 0.5) 
}

// Converts a center-based rect in normalized coords to a top-left-based rect in screen pixel coords,
// respecting the virtual square game area.
fn c_redimension_rect(x f64, y f64, w f64, h f64) (f32, f32, f32, f32) {
	// Calculate top-left corner in normalized coordinates first
	tl_x := x - (w / 2.0)
	tl_y := y + (h / 2.0)

	// Convert top-left position to screen coordinates using the corrected function
	gx, gy := c_redimension_coord(tl_x, tl_y)

	// Scale width and height directly using the virtual game area's dimensions
	gw := f32(w * gapi_game_w__)
	gh := f32(h * gapi_game_h__)
	
	return gx, gy, gw, gh
}

fn c_color(r u8, g u8, b u8, a u8) gg.Color {
	return gg.rgba(r, g, b, a)
}

fn get_time() f64 {
	return f64(time.now().unix_micro()) / f64(1_000_000)
}

@[export: 'gapi_log']
@[unsafe]
fn gapi_log(message &char) {
	println(term.gray('[log]: ${message.vstring()}'))
}