module vgama

#flag -I @VMODROOT/../lib

#flag -DMINIAUDIO_IMPLEMENTATION
// Miniaudio C Interop
#include <miniaudio.h>

@[typedef]
struct C.ma_engine {}

@[typedef]
struct C.ma_engine_config {}

@[typedef]
struct C.ma_sound {}

pub enum MaResult {
	success = 0
	error   = -1 // A generic error.
}

// Declared in vgama.v, but need it here too to access the global engine
pub fn C.ma_engine_init(pConfig &C.ma_engine_config, ppEngine &&C.ma_engine) MaResult
pub fn C.ma_engine_uninit(pEngine &C.ma_engine)

// Miniaudio Sound API
pub fn C.ma_sound_init_from_data_source(pEngine &C.ma_engine, pDataSource &C.ma_data_source, flags u32, pDataSourceGroup &C.ma_data_source_group, pSound &&C.ma_sound) MaResult
pub fn C.ma_sound_init_from_file(pEngine &C.ma_engine, pFilePath &char, flags u32, pDataSourceGroup &C.ma_data_source_group, pSound &&C.ma_sound) MaResult
pub fn C.ma_sound_init_from_memory(pEngine &C.ma_engine, pData &C.void, dataSize u64, flags u32, pDataSourceGroup &C.ma_data_source_group, pSound &&C.ma_sound) MaResult

pub fn C.ma_sound_uninit(pSound &C.ma_sound)
pub fn C.ma_sound_start(pSound &C.ma_sound) MaResult
pub fn C.ma_sound_stop(pSound &C.ma_sound) MaResult
pub fn C.ma_sound_set_looping(pSound &C.ma_sound, isLooping bool)
pub fn C.ma_sound_is_playing(pSound &C.ma_sound) bool
