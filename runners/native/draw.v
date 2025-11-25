module vgama

import gg

@[export: 'gapi_draw_rect']
fn gapi_draw_rect(x f64, y f64, w f64, h f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx, gy, gw, gh, c] () {
		gapi_ctx__.draw_rect_filled(gx, gy, gw, gh, c)
	}
	return 0
}

@[export: 'gapi_draw_rounded_rect']
fn gapi_draw_rounded_rect(x f64, y f64, w f64, h f64, r f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	gr := c_redimension_one(r)
	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx, gy, gw, gh, gr, c] () {
		gapi_ctx__.draw_rounded_rect_filled(gx, gy, gw, gh, gr, c)
	}
	return 0
}

@[export: 'gapi_draw_circle']
fn gapi_draw_circle(x f64, y f64, r f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy := c_redimension_coord(x, y)
	gr := c_redimension_one(r)
	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx, gy, gr, c] () {
		gapi_ctx__.draw_circle_filled(gx, gy, gr, c)
	}
	return 0
}

@[export: 'gapi_draw_ellipse']
fn gapi_draw_ellipse(x f64, y f64, w f64, h f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx, gy, gw, gh, c] () {
		gapi_ctx__.draw_ellipse_filled(gx, gy, gw, gh, c)
	}
	return 0
}

@[export: 'gapi_draw_triangle']
fn gapi_draw_triangle(x1 f64, y1 f64, x2 f64, y2 f64, x3 f64, y3 f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx1, gy1 := c_redimension_coord(x1, y1)
	gx2, gy2 := c_redimension_coord(x2, y2)
	gx3, gy3 := c_redimension_coord(x3, y3)

	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx1, gy1, gx2, gy2, gx3, gy3, c] () {
		gapi_ctx__.draw_triangle_filled(gx1, gy1, gx2, gy2, gx3, gy3, c)
	}
	return 0
}

@[export: 'gapi_draw_line']
fn gapi_draw_line(x1 f64, y1 f64, x2 f64, y2 f64, t f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx1, gy1 := c_redimension_coord(x1, y1)
	gx2, gy2 := c_redimension_coord(x2, y2)

	gt := c_redimension_one(t)

	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx1, gy1, gx2, gy2, gt, c] () {
		gapi_ctx__.draw_line_with_config(gx1, gy1, gx2, gy2, gg.PenConfig{
			color:     c
			thickness: gt
		})
	}
	return 0
}

const text_style_bold__ = u8(1)
const text_style_mono__ = u8(1 << 1)
const text_style_italic__ = u8(1 << 2)

@[export: 'gapi_draw_text']
@[unsafe]
fn gapi_draw_text(x f64, y f64,
	height f64,
	txt &char, font &char,
	style u8, align char,
	cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, _, gh := c_redimension_rect(x, y, 0, height)
	vtext := txt.vstring()
	vfont := font.vstring()
	italic := (style & text_style_italic__) > 0
	bold := (style & text_style_bold__) > 0
	mono := (style & text_style_mono__) > 0

	c := c_color(cr, cg, cb, ca)

	gapi_queue__ <- fn [gx, gy, gh, vtext, vfont, italic, bold, mono, c, align] () {
		gapi_ctx__.draw_text2(gg.DrawTextParams{
			x:         i32(gx)
			y:         i32(gy)
			text:      vtext
			color:     c
			size:      i32(gh)
			align:     if align == `l` {
				.left
			} else if align == `c` {
				.center
			} else {
				.right
			}
			max_width: gapi_width__
			family:    vfont
			bold:      bold
			mono:      mono
			italic:    italic
		})
	}
	return 0
}
