module vgama

import term
// Forward declaration of the global audio engine from vgama.v

// The stored audio data will now be a miniaudio sound object.
// We map them by a handle (ID).
__global (
	gapi_sounds__        map[u32]&C.ma_sound
	gapi_sound_count__   u32
	gapi_audio_engine__  &C.ma_engine // Global miniaudio engine instance
	gapi_engine_config__ C.ma_engine_config
)

@[export: 'gapi_create_audio']
@[unsafe]
fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) u32 {
	if gapi_audio_engine__ == unsafe { nil } {
		println(term.fail_message('[vgama.audio] Miniaudio engine not initialized. Cannot create audio.'))
		return 0
	}

	// Create a decoder for the raw PCM data (miniaudio expects a decoder or file/memory source)
	// We need to pass the raw float data for miniaudio to manage.
	// For raw PCM, we directly initialize a sound from memory.
	// Miniaudio's ma_sound_init_from_memory expects a specific format (ma_format_f32).
	// We are receiving f32 data, so this should work.

	mut sound := &C.ma_sound(nil)
	res := C.ma_sound_init_from_memory(gapi_audio_engine__, voidptr(data), frame_count * u64(channels) * u64(sizeof(f32)), // Total size in bytes
	 0, // Flags
	 nil, // pDataSourceGroup
	 &sound // Output sound pointer
	 )
	if res != .success {
		println(term.fail_message('[vgama.audio] Failed to create ma_sound from memory: ${res}.'))
		return 0
	}

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
			println(term.fail_message('[vgama.audio] Failed to start ma_sound: ${res}.'))
			return 1
		}
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to play unknown audio handle: ${handle}.'))
	return 1
}

@[export: 'gapi_stop_audio']
fn gapi_stop_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		res := C.ma_sound_stop(sound)
		if res != .success {
			println(term.fail_message('[vgama.audio] Failed to stop ma_sound: ${res}.'))
			return 1
		}
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to stop unknown audio handle: ${handle}.'))
	return 1
}

@[export: 'gapi_free_audio']
fn gapi_free_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		// Stop if currently playing
		if C.ma_sound_is_playing(sound) {
			C.ma_sound_stop(sound)
		}
		C.ma_sound_uninit(sound)
		gapi_sounds__.delete(handle)
		println(term.ok_message('[vgama.audio] Freed audio handle: ${handle}.'))
		return 0
	}
	println(term.warn_message('[vgama.audio] Attempted to free unknown audio handle: ${handle}.'))
	return 1
}

fn audio_deinit() {
	C.ma_engine_uninit(gapi_audio_engine__)
	println(term.ok_message('[vgama] Miniaudio engine uninitialized.'))
}

@[unsafe]
fn audio_init() {
	gapi_audio_engine__ = &C.ma_engine(nil) // Initialize to nil
	res := C.ma_engine_init(&gapi_engine_config__, &gapi_audio_engine__)
	if res != .success {
		println(term.fail_message('[vgama] Failed to initialize miniaudio engine.'))
	}
	println(term.ok_message('[vgama] Miniaudio engine initialized.'))
}
