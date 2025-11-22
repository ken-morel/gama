module vgama

@[export: 'gapi_key_pressed']
fn gapi_key_pressed(kt char, kk char) i32 {
	code := rune(kt).str() + rune(kk).str()
	return if code in gapi_pressed_keys__ { i32(1) } else { i32(0) }
}
