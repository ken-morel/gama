#ifndef GM_AUDIO_H_INCLUDED
#define GM_AUDIO_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h> // For malloc and free

#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio.h"

// For gm_ functions if GAPI is enabled
#include "gapi.h"
#include <stdbool.h>

// Represents raw, decoded audio data in memory.
typedef struct {
  float *data;
  ma_uint64 n_frames;
  ma_uint32 n_channels;
  ma_uint32 sample_rate;
} gmAudioData;

// Provides a consistent decoder configuration.
static inline ma_decoder_config gm_ma_decoder_config() {
  return ma_decoder_config_init(ma_format_f32, 2, 48000);
}

// Decodes an in-memory audio file into a gmAudioData struct.
int gm_audio_data_load_from_memory(gmAudioData *audio_data,
                                   const unsigned char *buffer, size_t len);

// Decodes an audio file from a path into a gmAudioData struct.
int gm_audio_data_load(gmAudioData *audio_data, const char *path);

// Frees the memory allocated for audio data.
void gm_audio_data_free(gmAudioData *audio_data);


// --- High-Level GAPI-dependent API ---

#ifndef GM_NO_GAPI

/**
 * @brief Loads an audio file from a path and prepares it for playback.
 * 
 * This function decodes the entire audio file into memory, sends the data
 * to the backend, and then frees the CPU-side memory.
 * 
 * @param path The file path to the audio file (e.g., .wav, .ogg, .mp3).
 * @return A gmAudio object. The handle will be 0 if loading fails.
 */
static inline gmAudio gm_load_audio(const char* path) {
  gmAudioData audio_data;
  if (gm_audio_data_load(&audio_data, path) != 0) {
    return (gmAudio){0}; 
  }

  uint32_t handle = gapi_create_audio(audio_data.data, audio_data.n_frames, audio_data.n_channels, audio_data.sample_rate);
  
  // Free the intermediate CPU-side data buffer now that it's in the backend.
  gm_audio_data_free(&audio_data);

  return (gmAudio){handle};
}

/**
 * @brief Plays a loaded audio file.
 * 
 * @param audio The gmAudio object to play.
 * @param loop If true, the sound will play continuously until stopped.
 */
static inline void gm_play_audio(gmAudio audio, bool loop) {
  if (audio.handle > 0) {
    gapi_play_audio(audio.handle, loop);
  }
}

/**
 * @brief Stops a playing audio file.
 * 
 * @param audio The gmAudio object to stop.
 */
static inline void gm_stop_audio(gmAudio audio) {
  if (audio.handle > 0) {
    gapi_stop_audio(audio.handle);
  }
}

/**
 * @brief Frees a loaded audio resource from the engine.
 * 
 * Call this when you are finished with a sound to free up resources.
 * 
 * @param audio The gmAudio object to free.
 */
static inline void gm_free_audio(gmAudio audio) {
  if (audio.handle > 0) {
    gapi_free_audio(audio.handle);
  }
}

#endif // GM_NO_GAPI


// --- Implementation ---

int gm_audio_data_load_from_memory(gmAudioData *audio_data,
                                   const unsigned char *buffer, size_t len) {
  ma_decoder decoder;
  ma_decoder_config config = gm_ma_decoder_config();

  ma_result result = ma_decoder_init_memory(buffer, len, &config, &decoder);
  if (result != MA_SUCCESS) {
    return -1; // Failed to initialize decoder
  }

  // Get total frame count and allocate memory.
  ma_decoder_get_length_in_pcm_frames(&decoder, &audio_data->n_frames);
  audio_data->data = (float *)malloc(audio_data->n_frames * config.channels * sizeof(float));
  if (audio_data->data == NULL) {
    ma_decoder_uninit(&decoder);
    return -2; // Malloc failed
  }

  // Decode the entire audio file.
  ma_decoder_read_pcm_frames(&decoder, audio_data->data, audio_data->n_frames, NULL);

  // Store format info
  audio_data->n_channels = decoder.outputChannels;
  audio_data->sample_rate = decoder.outputSampleRate;

  ma_decoder_uninit(&decoder);
  return 0;
}

int gm_audio_data_load(gmAudioData *audio_data, const char *path) {
  ma_decoder decoder;
  ma_decoder_config config = gm_ma_decoder_config();

  ma_result result = ma_decoder_init_file(path, &config, &decoder);
  if (result != MA_SUCCESS) {
    return -1; // Failed to initialize decoder
  }

  // Get total frame count and allocate memory.
  ma_decoder_get_length_in_pcm_frames(&decoder, &audio_data->n_frames);
  audio_data->data = (float *)malloc(audio_data->n_frames * config.channels * sizeof(float));
  if (audio_data->data == NULL) {
    ma_decoder_uninit(&decoder);
    return -2; // Malloc failed
  }

  // Decode the entire audio file.
  ma_decoder_read_pcm_frames(&decoder, audio_data->data, audio_data->n_frames, NULL);

  // Store format info
  audio_data->n_channels = decoder.outputChannels;
  audio_data->sample_rate = decoder.outputSampleRate;

  ma_decoder_uninit(&decoder);
  return 0;
}

void gm_audio_data_free(gmAudioData *audio_data) {
  if (audio_data && audio_data->data) {
    free(audio_data->data);
    audio_data->data = NULL;
    audio_data->n_frames = 0;
  }
}

#endif // GM_AUDIO_H_INCLUDED
