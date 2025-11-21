module main

import gg

@[export: 'gapi_draw_rect']
pub fn gapi_draw_rect(x f64, y f64, w f64, h f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	c := c_color(cr, cg, cb, ca)

	gapi_draw_queue__ << fn [gx, gy, gw, gh, c] () {
		gapi_ctx__.draw_rect(gg.DrawRectParams{
			x:          gx
			y:          gy
			w:          gw
			h:          gh
			color:      c
			is_rounded: false
			radius:     0
		})
	}
	return 0
}

@[export: 'gapi_draw_rounded_rect']
pub fn gapi_draw_rounded_rect(x f64, y f64, w f64, h f64, r f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	gr := c_redimension_one(r)
	c := c_color(cr, cg, cb, ca)

	gapi_draw_queue__ << fn [gx, gy, gw, gh, gr, c] () {
		gapi_ctx__.draw_rect(gg.DrawRectParams{
			x:          gx
			y:          gy
			w:          gw
			h:          gh
			color:      c
			is_rounded: true
			radius:     gr
		})
	}
	return 0
}
