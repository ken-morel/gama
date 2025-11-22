module vgama

import gg

fn ngl(x f64) f64 {
	return (x + 1) / 2
}

fn c_redimension_rect(x f64, y f64, w f64, h f64) (f32, f32, f32, f32) {
	gx, gy := c_redimension_coord(x - (w / 2), y - (h / 2))
	gw, gh := c_redimension_coord(w, h)
	return gx, gy, gw, gh
}

fn c_redimension_coord(x f64, y f64) (f32, f32) {
	return c_redimension_one(x), c_redimension_one(y)
}

fn c_redimension_one(v f64) f32 {
	return f32(ngl(v) * gapi_side__)
}

fn c_color(r u8, g u8, b u8, a u8) gg.Color {
	return gg.rgba(r, g, b, a)
}
