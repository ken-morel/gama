module vgama

import gg
import time
import term

fn c_redimension_coord(x f64, y f64) (f32, f32) {
	norm_x := (x + 1.0) * 0.5
	norm_y := (1.0 - y) * 0.5 // Invert Y-axis for screen coordinates

	return f32(norm_x * gapi_game_w__ + gapi_offset_x__), f32(norm_y * gapi_game_h__ +
		gapi_offset_y__)
}

fn v_redimension_coord(x f32, y f32) (f64, f64) {
	norm_x := (x - gapi_offset_x__) / gapi_game_w__
	norm_y := (y - gapi_offset_y__) / gapi_game_h__

	return (norm_x * 2) - 1.0, 1.0 - (norm_y * 2)
}

fn c_redimension_one(v f64) f32 {
	return f32(v * gapi_game_w__ * 0.5)
}

fn v_redimension_one(v f32) f64 {
	return (v * 2) / gapi_game_w__
}

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
