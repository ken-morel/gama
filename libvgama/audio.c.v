module vgama

#flag -I @VMODROOT/../lib

// Configure miniaudio for Linux with engine functionality
#flag -DMA_ENABLE_PLAYBACK
#flag -DMA_ENABLE_CAPTURE
#flag -DMA_ENABLE_DUPLEX
#flag -DMA_ENABLE_RESOURCE_MANAGER
#flag -DMA_ENABLE_ENGINE
#flag -DMA_ENABLE_EFFECTS
#flag -DMA_ENABLE_SDL
#flag -DMA_ENABLE_ALSA
#flag -DMA_ENABLE_PULSEAUDIO
#flag -DMA_ENABLE_JACK
#flag -DMA_ENABLE_NULL
#flag -DMINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// --- V-to-C Enums and Constants ---
pub enum MaResult {
	success                       = 0
	error                         = -1 // A generic error.
	invalid_args                  = -2
	invalid_operation             = -3
	out_of_memory                 = -4
	out_of_range                  = -5
	access_denied                 = -6
	does_not_exist                = -7
	already_exists                = -8
	too_many_open_files           = -9
	invalid_file                  = -10
	too_big                       = -11
	path_too_long                 = -12
	name_too_long                 = -13
	not_directory                 = -14
	is_directory                  = -15
	directory_not_empty           = -16
	at_end                        = -17
	no_space                      = -18
	busy                          = -19
	io_error                      = -20
	interrupt                     = -21
	unavailable                   = -22
	already_in_use                = -23
	bad_address                   = -24
	bad_seek                      = -25
	bad_pipe                      = -26
	deadlock                      = -27
	too_many_links                = -28
	not_implemented               = -29
	no_message                    = -30
	bad_message                   = -31
	no_data_available             = -32
	invalid_data                  = -33
	timeout                       = -34
	no_network                    = -35
	not_unique                    = -36
	not_socket                    = -37
	no_address                    = -38
	bad_protocol                  = -39
	protocol_unavailable          = -40
	protocol_not_supported        = -41
	protocol_family_not_supported = -42
	address_family_not_supported  = -43
	socket_not_supported          = -44
	connection_reset              = -45
	already_connected             = -46
	not_connected                 = -47
	connection_refused            = -48
	no_host                       = -49
	in_progress                   = -50
	cancelled                     = -51
	memory_already_mapped         = -52

	// General non-standard errors.
	crc_mismatch = -100

	// General miniaudio-specific errors.
	format_not_supported      = -200
	device_type_not_supported = -201
	share_mode_not_supported  = -202
	no_backend                = -203
	no_device                 = -204
	api_not_found             = -205
	invalid_device_config     = -206
	loop                      = -207
	backend_not_enabled       = -208

	// State errors.
	device_not_initialized     = -300
	device_already_initialized = -301
	device_not_started         = -302
	device_not_stopped         = -303

	// Operation errors.
	failed_to_init_backend         = -400
	failed_to_open_backend_device  = -401
	failed_to_start_backend_device = -402
	failed_to_stop_backend_device  = -403
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
pub fn C.ma_engine_init(pConfig &C.ma_engine_config, ppEngine &C.ma_engine) MaResult
pub fn C.ma_engine_config_init() C.ma_engine_config
pub fn C.ma_engine_uninit(pEngine &C.ma_engine)

pub fn create_audio_engine() !C.ma_engine {
	engine := C.ma_engine{}
	config := C.ma_engine_config_init()
	result := C.ma_engine_init(&config, &engine)
	return if result == .success {
		engine
	} else {
		error('${result}')
	}
}

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
