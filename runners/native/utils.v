module vgama

import gg
import time
import term

fn c_redimension_coord(x f64, y f64) (f32, f32) {
	return f32((x + 1.0) * 0.5 * gapi_width__), f32((1.0 - y) * 0.5 * gapi_height__) // Y is inverted
}

fn c_redimension_one(v f64) f32 {
	return f32(v * gapi_side__ * 0.5)
}

// Converts a center-based rect in normalized coords to a top-left-based rect in screen pixel coords.
fn c_redimension_rect(x f64, y f64, w f64, h f64) (f32, f32, f32, f32) {
	// Calculate top-left corner in normalized coordinates first
	tl_x := x - (w / 2.0)
	tl_y := y + (h / 2.0)

	// Convert top-left position to screen coordinates
	gx, gy := c_redimension_coord(tl_x, tl_y)

	// Scale width and height directly to screen pixels, not as coordinates
	gw := f32(w * gapi_side__)
	gh := f32(h * gapi_side__)

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
