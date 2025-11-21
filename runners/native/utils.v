module main

import gg

fn c_redimension(x f64, y f64, w f64, h f64) (f32, f32, f32, f32) {
	return f32((x + 1) * gapi_width__), f32((y + 1) * gapi_height__), f32(w * gapi_width__), f32(h * gapi_height__)
}

fn c_color(r u8, g u8, b u8, _ u8) gg.Color {
	return gg.rgb(r, g, b)
}
