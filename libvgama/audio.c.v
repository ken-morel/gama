module vgama

#flag -I @VMODROOT/../lib

#flag -DMINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// --- V-to-C Enums and Constants ---
pub enum MaResult {
	success = 0
	error   = -1
}
pub const ma_format_f32 = 3 // Corresponds to `ma_format_f32`

// --- C Struct Definitions for V ---
@[typedef]
struct C.ma_engine {}

@[typedef]
struct C.ma_engine_config {}

@[typedef]
struct C.ma_sound {}

@[typedef]
struct C.ma_decoder {}

@[typedef]
struct C.ma_decoder_config {}

// --- C Function Declarations for V ---

// Engine
pub fn C.ma_engine_init(pConfig &C.ma_engine_config, ppEngine &&C.ma_engine) MaResult
pub fn C.ma_engine_uninit(pEngine &C.ma_engine)

// Decoder (as a Data Source)
pub fn C.ma_decoder_config_init(outputFormat u32, outputChannels u32, outputSampleRate u32) C.ma_decoder_config
pub fn C.ma_decoder_init_memory(pData &u8, dataSize u64, pConfig &C.ma_decoder_config, ppDecoder &&C.ma_decoder) MaResult
pub fn C.ma_decoder_uninit(pDecoder &C.ma_decoder)

// Sound
pub fn C.ma_sound_init_from_data_source(pEngine &C.ma_engine, pDataSource &C.void, flags u32, pGroup &C.void, ppSound &&C.ma_sound) MaResult
pub fn C.ma_sound_uninit(pSound &C.ma_sound)
pub fn C.ma_sound_start(pSound &C.ma_sound) MaResult
pub fn C.ma_sound_stop(pSound &C.ma_sound) MaResult
pub fn C.ma_sound_set_looping(pSound &C.ma_sound, isLooping bool)
