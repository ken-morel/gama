module vgama

import sokol.audio

type SaudioDesc = C.saudio_desc

@[heap]
struct GmAudio {
	pcm_data    []f32
	channels    i32
	sample_rate i32
}

__global (
	gapi_audios__      map[i32]GmAudio
	gapi_audio_count__ i32
)

@[export: 'gapi_create_audio']
@[unsafe]
fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) i32 {
	gapi_audio_count__ += 1
	pcm_data := []f32{cap: i32(frame_count)}
	for i in 0 .. frame_count {
		pcm_data << data[i]
	}
	gapi_audios__[gapi_audio_count__] = GmAudio{
		pcm_data:    pcm_data
		channels:    i32(channels)
		sample_rate: i32(sample_rate)
	}
	return gapi_audio_count__
}

@[export: 'gapi_play_audio']
fn gapi_play_audio(handle i32, repeat i32) i32 {
	if sound := gapi_audios__[handle] {
		audio.setup(num_channels: sound.channels, sample_rate: sound.sample_rate)
		audio.push(sound.pcm_data.data, sound.pcm_data.len)
		return 0
	} else {
		return 1
	}
}

@[export: 'gapi_stop_audio']
fn gapi_stop_audio(handle i32) i32 {
	return 0
}

@[export: 'gapi_free_audio']
fn gapi_free_audio(handle i32) i32 {
	return 0
}
