module vgama

import term

// Global state for the audio system
__global (
	gapi_sounds__       map[u32]&C.ma_sound
	gapi_decoders__     map[u32]&C.ma_decoder // We MUST keep the decoder alive while the sound uses it
	gapi_sound_count__  u32
	gapi_audio_engine__ &C.ma_engine
)

// Initializes the miniaudio engine.
@[unsafe]
pub fn audio_init() {
	gapi_audio_engine__ = &C.ma_engine(nil)
	res := C.ma_engine_init(unsafe { nil }, &gapi_audio_engine__)
	if res != .success {
		println(term.fail_message('[vgama] Failed to initialize miniaudio engine: ${res}'))
		return
	}
	println(term.ok_message('[vgama] Miniaudio engine initialized.'))
}

// Uninitializes the miniaudio engine and frees all sound resources.
pub fn audio_deinit() {
	for _, sound in gapi_sounds__ {
		C.ma_sound_uninit(sound)
	}
	for _, decoder in gapi_decoders__ {
		C.ma_decoder_uninit(decoder)
	}
	gapi_sounds__.clear()
	gapi_decoders__.clear()

	C.ma_engine_uninit(gapi_audio_engine__)
	println(term.ok_message('[vgama] Miniaudio engine uninitialized.'))
}

@[export: 'gapi_create_audio']
@[unsafe]
fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) u32 {
	if gapi_audio_engine__ == unsafe { nil } {
		println(term.fail_message('[vgama.audio] Miniaudio engine not initialized.'))
		return 0
	}

	// 1. Create a decoder config for our raw PCM data
	decoder_config := C.ma_decoder_config_init(ma_format_f32, channels, sample_rate)

	// 2. Initialize a decoder from the in-memory PCM data
	mut decoder := &C.ma_decoder(nil)
	data_size_bytes := frame_count * u64(channels) * u64(sizeof(f32))
	res_decoder := C.ma_decoder_init_memory(unsafe { &u8(data) }, data_size_bytes, &decoder_config,
		&decoder)
	if res_decoder != .success {
		println(term.fail_message('[vgama.audio] Failed to init decoder from memory: ${res_decoder}'))
		return 0
	}

	// 3. Initialize a sound from the decoder (which acts as a data source)
	mut sound := &C.ma_sound(nil)
	res_sound := C.ma_sound_init_from_data_source(gapi_audio_engine__, unsafe { &C.void(decoder) },
		0, unsafe { nil }, &sound)
	if res_sound != .success {
		println(term.fail_message('[vgama.audio] Failed to init sound from data source: ${res_sound}'))
		C.ma_decoder_uninit(decoder) // Clean up the decoder if sound init fails
		return 0
	}

	// 4. Store both the sound and the decoder
	gapi_sound_count__ += 1
	gapi_sounds__[gapi_sound_count__] = sound
	gapi_decoders__[gapi_sound_count__] = decoder

	return gapi_sound_count__
}

@[export: 'gapi_play_audio']
fn gapi_play_audio(handle u32, loop_flag i32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_set_looping(sound, loop_flag != 0)
		res := C.ma_sound_start(sound)
		return if res == .success { 0 } else { 1 }
	}
	return 1
}

@[export: 'gapi_stop_audio']
fn gapi_stop_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_stop(sound)
		return 0
	}
	return 1
}

@[export: 'gapi_free_audio']
fn gapi_free_audio(handle u32) i32 {
	if sound := gapi_sounds__[handle] {
		C.ma_sound_uninit(sound)
		gapi_sounds__.delete(handle)

		if decoder := gapi_decoders__[handle] {
			C.ma_decoder_uninit(decoder)
			gapi_decoders__.delete(handle)
		}
		return 0
	}
	return 1
}
