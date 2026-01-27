module vgama

import sokol.audio

type SaudioDesc = C.saudio_desc

@[heap]
struct GmAudio {
	data        &f32
	frame_count u64
	channels    i32
	sample_rate i32
}

__global (
	gapi_audios__      map[u32]GmAudio
	gapi_audio_count__ u32
)

fn audio_init() {
	gapi_audio_count__ = 0
	gapi_audios__ = map[u32]GmAudio{}
}

fn audio_player(a &GmAudio, index u64) {
	desc := SaudioDesc{
		sample_rate:  a.sample_rate
		num_channels: a.channels
		stream_cb:    fn (buffer &f32, num_frames int, num_channels int) {
			unsafe {
				// mut b := buffer
				// for i, sample in samples {
				// 	b[i] = sample
				// }
			}
		}
	}
	audio.setup(desc)
}

@[export: 'gapi_create_audio']
pub fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) i32 {
	gapi_audio_count__ += 1
	gapi_audios__[gapi_audio_count__] = GmAudio{
		data:        data
		frame_count: frame_count
		channels:    i32(channels)
		sample_rate: i32(sample_rate)
	}
	return i32(gapi_audio_count__)
}
