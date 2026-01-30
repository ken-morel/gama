module vgama

import term

// Miniaudio C Interop - Define all necessary types and functions

__global (
	gapi_sounds__           map[u32]&C.ma_sound
	gapi_sound_count__      u32
	gapi_audio_engine__     &C.ma_engine
	gapi_resource_manager__ &C.ma_resource_manager
)

@[export: 'gapi_create_audio']
@[unsafe]
fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) u32 {
	if gapi_audio_engine__ == unsafe { nil } {
		println(term.fail_message('[vgama.audio] Miniaudio engine not initialized.'))
		return 0
	}

	// 1. Create a data buffer resource from the raw PCM data
	mut data_buffer := &C.ma_resource_manager_data_buffer(nil)
	res_buffer := C.ma_resource_manager_data_buffer_init(gapi_resource_manager__, unsafe { voidptr(data) },
		frame_count * u64(channels) * u64(sizeof(f32)), &data_buffer)
	if res_buffer != .success {
		println(term.fail_message('[vgama.audio] Failed to create data buffer resource: ${res_buffer}'))
		return 0
	}

	// 2. Initialize a sound from the data buffer (which is a data source)
	mut sound := &C.ma_sound(nil)
	res_sound := C.ma_sound_init_from_data_source(gapi_audio_engine__, &C.ma_data_source(data_buffer),
		0, unsafe { nil }, &sound)
	if res_sound != .success {
		println(term.fail_message('[vgama.audio] Failed to init sound from data source: ${res_sound}'))
		// In a real scenario, you would uninit the data_buffer here
		return 0
	}

	// 3. Store the sound
	gapi_sound_count__ += 1
	gapi_sounds__[gapi_sound_count__] = sound
	println(term.ok_message('[vgama.audio] Created audio handle: ${gapi_sound_count__}'))
	return gapi_sound_count__
}

@[export: 'gapi_play_audio']
fn gapi_play_audio(handle u32, loop_flag i32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_set_looping(sound, loop_flag != 0)
		res := C.ma_sound_start(sound)
		if res != .success {
			println(term.fail_message('[vgama.audio] Failed to start sound: ${res}'))
			return 1
		}
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to play unknown audio handle: ${handle}'))
	return 1
}

@[export: 'gapi_stop_audio']
fn gapi_stop_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_stop(sound)
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to stop unknown audio handle: ${handle}'))
	return 1
}

@[export: 'gapi_free_audio']
fn gapi_free_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_uninit(sound) // This also uninitializes the underlying data source
		gapi_sounds__.delete(handle)
		println(term.ok_message('[vgama.audio] Freed audio handle: ${handle}'))
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to free unknown audio handle: ${handle}'))
	return 1
}

fn audio_deinit() {
	C.ma_resource_manager_uninit(gapi_resource_manager__)
	C.ma_engine_uninit(gapi_audio_engine__)
	println(term.ok_message('[vgama] Miniaudio engine and resource manager uninitialized.'))
}

@[unsafe]
fn audio_init() {
	// Init Engine
	mut engine_config := &C.ma_engine_config(nil)
	gapi_audio_engine__ = &C.ma_engine(nil)
	res_engine := C.ma_engine_init(engine_config, &gapi_audio_engine__)
	if res_engine != .success {
		println(term.fail_message('[vgama] Failed to initialize miniaudio engine: ${res_engine}'))
		return
	}

	// Init Resource Manager
	mut rm_config := C.ma_resource_manager_config{} // TODO: Set this up properly
	gapi_resource_manager__ = &C.ma_resource_manager(nil)
	res_rm := C.ma_resource_manager_init(&rm_config, &gapi_resource_manager__)
	if res_rm != .success {
		println(term.fail_message('[vgama] Failed to initialize resource manager: ${res_rm}'))
		C.ma_engine_uninit(gapi_audio_engine__) // Clean up engine if rm fails
		return
	}

	println(term.ok_message('[vgama] Miniaudio engine and resource manager initialized.'))
}
