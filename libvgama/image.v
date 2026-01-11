module vgama

import gg
import term
import stbi
import os

@[export: 'gapi_create_image']
@[unsafe]
fn gapi_create_image(data &u8, width u32, height u32) u32 {
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
fn gapi_draw_image(handle u32, x f64, y f64, w f64, h f64) {
	if handle == 0 {
		return
	}
	gx, gy, gw, gh := c_redimension_rect(x, y, w, h)
	queue_fn(fn [gx, gy, gw, gh, handle] () {
		if img := gapi_images__[handle] {
			gapi_ctx__.draw_image(gx, gy, gw, gh, &img)
		} else {
			println(term.warn_message('Image ${handle} does not exist'))
		}
	})
}

@[export: 'gapi_draw_image_part']
fn gapi_draw_image_part(handle u32, sx u32, sy u32, sw u32, sh u32, x f64, y f64, w f64, h f64) u32 {
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
