module vgama

import gg
import term
import stbi
import os
import sokol.sapp

@[export: 'gapi_create_image']
@[unsafe]
fn gapi_create_image(data &u8, width u32, height u32) i32 {
	gapi_image_count__ += 1
	idx := gapi_image_count__ - 1

	path := os.join_path(gapi_dir__, 'image-${idx}.bmp')

	stbi.stbi_write_bmp(path, int(width), int(height), 4, data) or {
		println(term.warn_message('Failed to cache image(${idx}:${width}x${height}) data to file ${path}: ${err}'))
		return 0
	}

	img := gapi_ctx__.create_image(path) or {
		println(term.warn_message('Failed to create gg image from cached file: ${err}'))
		return 0
	}

	gapi_images__[idx] = img
	return idx
}

@[export: 'gapi_draw_image']
fn gapi_draw_image(handle i32, x f64, y f64, w f64, h f64) {
	if handle == 0 {
		return
	}
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	queue_fn(fn [gx, gy, gw, gh, handle] () {
		mut w := gw
		mut h := gh
		mut x := gx
		mut y := gy
		if w == 0 && h == 0 {
			w = gapi_width__
			h = gapi_height__
			x = 0
			y = 0
		}
		if img := gapi_images__[handle] {
			gapi_ctx__.draw_image(x, y, w, h, &img)
		} else {
			println(term.warn_message('Image ${handle} does not exist'))
		}
	})
}

@[export: 'gapi_draw_image_part']
fn gapi_draw_image_part(handle i32, sx u32, sy u32, sw u32, sh u32, x f64, y f64, w f64, h f64) u32 {
	if handle == 0 {
		return 0
	}
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	img_r := gg.Rect{
		x:      gx
		y:      gy
		width:  gw
		height: gh
	}
	part_r := gg.Rect{
		x:      f32(sx)
		y:      f32(sy)
		width:  f32(sw)
		height: f32(sh)
	}
	queue_fn(fn [img_r, part_r, handle] () {
		if img := gapi_images__[handle] {
			gapi_ctx__.draw_image_part(img_r, part_r, &img)
		} else {
			println(term.warn_message('Image ${handle} does not exist'))
		}
	})
	return 0
}

@[export: 'gapi_snap']
@[unsafe]
fn gapi_snap(handle i32, width &u32, height &u32) i32 {
	mut ret := -3
	mut retptr := &ret
	gapi_wait_queue()
	queue_fn(fn [handle, width, height, mut retptr] () {
		path := os.join_path(gapi_dir__, 'screenshot${handle}.png')
		sapp.screenshot_png(path) or {
			println(term.fail_message('Error screenshoting to ${path}: ${err}'))
			unsafe {
				*retptr = -1
			}
			return
		}
		img := gapi_ctx__.create_image(path) or {
			println(term.warn_message('Failed to create gg image from cached file: ${err}'))
			unsafe {
				*retptr = -2
			}
			return
		}
		gapi_images__[handle] = img
		unsafe {
			*retptr = 0
		}
		println(term.ok_message('[vgama]: Snapped ${handle} to ${path}'))
	})
	gapi_wait_queue()
	return ret
}

@[export: 'gapi_clear']
fn gapi_clear() i32 {
	queue_fn(fn () {
		gapi_ctx__.end(how: .clear)
	})
	return 0
}
