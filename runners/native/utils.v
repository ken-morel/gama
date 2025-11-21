module main

import gg

fn ngl(x f64) f64 {
	return (x + 1) / 2
}

fn c_redimension_rect(x f64, y f64, w f64, h f64) (f32, f32, f32, f32) {
	return f32(ngl(x - w / 2) * gapi_width__), f32(ngl(y - h / 2) * gapi_height__), f32(w * gapi_width__ / 2), f32(h * gapi_height__ / 2)
}

fn c_redimension_one(x f64) f32 {
	return (x / 2) * gapi_side__
}

fn c_color(r u8, g u8, b u8, _ u8) gg.Color {
	return gg.rgb(r, g, b)
}
