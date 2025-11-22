module vgama

import term

@[export: 'gapi_create_image']
@[unsafe]
fn gapi_create_image(path &char, width &u32, height &u32) u32 {
	vpath := path.vstring()
	gapi_image_count__ += 1
	idx := gapi_image_count__ - 1
	img := gapi_ctx__.create_image(vpath) or {
		println(term.fail_message('[vgama]: Error loading image ${vpath}: ${err}'))
		return 0
	}
	gapi_images__[idx] = img
	if width != nil {
		*width = u32(img.width)
	}
	if height != nil {
		*height = u32(img.height)
	}
	return idx
}

@[export: 'gapi_draw_image']
fn gapi_draw_image(handle u32, x f64, y f64, w f64, h f64) {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	gapi_queue__ << fn [gx, gy, gw, gh, handle] () {
		if img := gapi_images__[handle] {
			gapi_ctx__.draw_image(gx, gy, gw, gh, &img)
		} else {
			println(term.warn_message('Image ${handle} does not exist'))
		}
	}
}

@[export: 'gapi_draw_image_part']
fn gapi_draw_image_part(handle u32, sx u32, sy u32, sw u32, sh u32, x f64, y f64, w f64, h f64) u32 {
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	img_r := gg.Rect{
		x: gx
		y: gy
		width: gw
		height: gh
	}
	part_r := gg.Rect{
		x: f32(sx)
		y: f32(sy)
		width: f32(sw)
		height: f32(sh)
	}
	gapi_queue__ << fn [img_r, part_r, handle] () {
		if img := gapi_images__[handle] {
			gapi_ctx__.draw_image_part(gx, , &img)
		} else {
			println(term.warn_message('Image ${handle} does not exist'))
		}
	}
}
