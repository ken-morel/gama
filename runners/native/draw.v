module main

import gg

@[export: 'gapi_draw_rect']
pub fn gapi_draw_rect(x f64, y f64, w f64, h f64, cr u8, cg u8, cb u8, ca u8) i32 {
	gx, gy, gw, gh := c_redimension(x, y, w, h)
	c := c_color(cr, cg, cb, ca)
	// gapi_ctx__.draw_rect(gg.DrawRectParams{
	// 	x:          10
	// 	y:          10
	// 	w:          10
	// 	h:          10
	// 	color:      gg.blue
	// 	is_rounded: false
	// 	radius:     0
	// })

	// gapi_ctx__.draw_convex_poly([f32(100.0), 100.0, 200.0, 100.0, 300.0, 200.0, 200.0, 300.0, 100.0,
	// 	300.0], gg.blue)
	// gapi_ctx__.draw_poly_empty([f32(50.0), 50.0, 70.0, 60.0, 90.0, 80.0, 70.0, 110.0],
	// 	gg.black)
	gapi_ctx__.draw_triangle_filled(450, 142, 530, 280, 370, 280, gg.red)

	// gapi_ctx__.draw_convex_poly([f32(100.0), 100.0, 200.0, 100.0, 300.0, 200.0, 200.0, 300.0, 100.0,
	// 	300.0], gg.blue)
	// gapi_ctx__.draw_poly_empty([f32(50.0), 50.0, 70.0, 60.0, 90.0, 80.0, 70.0, 110.0],
	// 	gg.black)
	// gapi_ctx__.draw_triangle_filled(450, 142, 530, 280, 370, 280, gg.red)

	// gapi_ctx__.draw_rect(gg.DrawRectParams{
	// 	x:          gx
	// 	y:          gy
	// 	w:          gw
	// 	h:          gh
	// 	color:      c
	// 	is_rounded: false
	// 	radius:     0
	// })
	return 0
}
