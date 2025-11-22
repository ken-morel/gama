module main

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
