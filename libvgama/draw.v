module vgama

import gg

@[export: 'gapi_draw_rect']
fn gapi_draw_rect(x f64, y f64, w f64, h f64, col GmColor) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	c := col.to_gg()

	queue_fn(fn [gx, gy, gw, gh, c] () {
		gapi_ctx__.draw_rect_filled(gx, gy, gw, gh, c)
	})
	return 0
}

@[export: 'gapi_draw_rounded_rect']
fn gapi_draw_rounded_rect(x f64, y f64, w f64, h f64, r f64, col GmColor) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	gr := c_redimension_one(r)
	c := col.to_gg()

	queue_fn(fn [gx, gy, gw, gh, gr, c] () {
		gapi_ctx__.draw_rounded_rect_filled(gx, gy, gw, gh, gr, c)
	})
	return 0
}

@[export: 'gapi_draw_circle']
fn gapi_draw_circle(x f64, y f64, r f64, col GmColor) i32 {
	gx, gy := c_redimension_coord(x, y)
	gr := c_redimension_one(r)
	c := col.to_gg()

	queue_fn(fn [gx, gy, gr, c] () {
		gapi_ctx__.draw_circle_filled(gx, gy, gr, c)
	})
	return 0
}

@[export: 'gapi_draw_ellipse']
fn gapi_draw_ellipse(x f64, y f64, w f64, h f64, col GmColor) i32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	c := col.to_gg()

	queue_fn(fn [gx, gy, gw, gh, c] () {
		gapi_ctx__.draw_ellipse_filled(gx, gy, gw, gh, c)
	})
	return 0
}

@[export: 'gapi_draw_triangle']
fn gapi_draw_triangle(x1 f64, y1 f64, x2 f64, y2 f64, x3 f64, y3 f64, col GmColor) i32 {
	gx1, gy1 := c_redimension_coord(x1, y1)
	gx2, gy2 := c_redimension_coord(x2, y2)
	gx3, gy3 := c_redimension_coord(x3, y3)

	c := col.to_gg()

	queue_fn(fn [gx1, gy1, gx2, gy2, gx3, gy3, c] () {
		gapi_ctx__.draw_triangle_filled(gx1, gy1, gx2, gy2, gx3, gy3, c)
	})
	return 0
}

@[export: 'gapi_draw_triangles']
@[unsafe]
fn gapi_draw_triangles(n u32, p &f64, c_cols &GmColor) {
	triangles := [][6]f32{len: i32(n)}
	cols := []gg.Color{len: i32(n)}
	for i in 0 .. int(n) {
		cols[i] = c_cols[i].to_gg()
		for j in 0 .. 3 {
			o := j * 2
			idx := i * 6 + o
			a, b := c_redimension_coord(p[idx + 0], p[idx + 1])
			triangles[i][o + 0] = a
			triangles[i][o + 1] = b
		}
	}
	queue_fn(fn [triangles, n, cols] () {
		for i in 0 .. int(n) {
			gapi_ctx__.draw_triangle_filled(triangles[i][0], triangles[i][1], triangles[i][2],
				triangles[i][3], triangles[i][4], triangles[i][5], cols[i])
		}
	})
}

@[export: 'gapi_draw_line']
fn gapi_draw_line(x1 f64, y1 f64, x2 f64, y2 f64, t f64, col GmColor) i32 {
	gx1, gy1 := c_redimension_coord(x1, y1)
	gx2, gy2 := c_redimension_coord(x2, y2)

	gt := c_redimension_one(t)

	c := col.to_gg()

	queue_fn(fn [gx1, gy1, gx2, gy2, gt, c] () {
		gapi_ctx__.draw_line_with_config(gx1, gy1, gx2, gy2, gg.PenConfig{
			color:     c
			thickness: gt
		})
	})
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
	style u8, col GmColor) i32 {
	gx, gy := c_redimension_coord(x, y)
	size := i32(c_redimension_one(height))
	vtext := txt.vstring().clone()
	vfont := font.vstring().clone()
	italic := (style & text_style_italic__) != 0
	bold := (style & text_style_bold__) != 0
	mono := (style & text_style_mono__) != 0

	c := col.to_gg()

	queue_fn(fn [gx, gy, size, vtext, vfont, italic, bold, mono, c] () {
		gapi_ctx__.draw_text(i32(gx), i32(gy), vtext, gg.TextCfg{
			color:          c
			size:           size
			align:          .center
			mono:           mono
			vertical_align: .middle
			italic:         italic
			bold:           bold
			family:         vfont
		})
	})
	return 0
}
