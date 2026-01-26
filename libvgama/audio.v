module vgama

import sokol.audio


type SaudioDesc = audio.C.saudio_desc

struct gmAudio {
	data        &f32
	frame_count u64
	channels    u32
	sample_rate u32
}

__global {
	gapi_audios__ map[u32]gmAudio
	gapi_audio_count__ u32
}

fn audio_init() {
	gapi_audio_count__ = 0
	gapi_audios__ = map[u32]gmAudio()
}

fn audio_player(a &gmAudio, index u64) {
	audio.C.saudio_desc{}
	desc := SaudioDesc{
		sample_rate:  a.sample_rate
		num_channels: a.channels
		stream_cb:    fn (buffer &f32, num_frames int, num_channels int) {
			unsafe {
				mut b := buffer
				for i, sample in samples {
					b[i] = sample
				}
			}
		}
	}
	audio.setup(desc)
}

@[export: 'gapi_create_audio']
pub fn gapi_create_audio(data &f32, frame_count u64, channels u32, sample_rate u32) i32 {
	gapi_audio_count__ += 1
	gapi_audios__[gapi_audio_count__] = gmAudio{
		data: data
		frame_count: frame_count
		channels: channels
		sample_rate: sample_rate
	}
}
