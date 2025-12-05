module vgama

@[export: 'gapi_key_pressed']
fn gapi_key_pressed(kt char, kk char) i32 {
	code := rune(kt).str() + rune(kk).str()
	gapi_wait_queue()
	return if code in gapi_pressed_keys__ { i32(1) } else { i32(0) }
}

@[export: 'gapi_get_mouse_move']
@[unsafe]
fn gapi_get_mouse_move(x &f64, y &f64) {
	vx, vy := v_redimension_coord(gapi_mouse_move_x__, gapi_mouse_move_y__)
	*x = vx
	*y = vy
}

@[export: 'gapi_mouse_down']
fn gapi_mouse_down() i32 {
	return if gapi_mouse_down__ { i32(1) } else { i32(0) }
}

@[export: 'gapi_mouse_pressed']
fn gapi_mouse_pressed() i32 {
	return if gapi_mouse_pressed__ { i32(1) } else { i32(0) }
}

@[export: 'gapi_mouse_get']
@[unsafe]
fn gapi_mouse_get(x &f64, y &f64) {
	vx, vy := v_redimension_coord(gapi_mouse_x__, gapi_mouse_y__)
	*x = vx
	*y = vy
}
