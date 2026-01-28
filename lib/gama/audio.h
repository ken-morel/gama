#ifndef GM_AUDIO_H_INCLUDED
#define GM_AUDIO_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h> // For malloc and free

#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_DEVICE_IO
#define MA_ENABLE_NULL
#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
// BUG: bug source, not knowing the use
#define MA_NO_RESOURCE_MANAGER
#define MA_NO_THREADING
#define MA_NO_ENGINE

#include "../miniaudio.h"

/**
 * @brief Opaque handle to an audio resource in the backend.
 */
typedef struct {
  uint32_t handle;
} gmAudio;

#include <stdbool.h>

// Represents raw, decoded audio data in memory.
typedef struct {
  ma_float *data;
  ma_uint64 n_frames;
  ma_uint32 n_channels;
  ma_uint32 sample_rate;
} gmAudioData;

// Provides a consistent decoder configuration.
static inline ma_decoder_config gm_ma_decoder_config() {
  return ma_decoder_config_init(ma_format_f32, 2, 48000);
}

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
  audio_data->data =
      (float *)malloc(audio_data->n_frames * config.channels * sizeof(float));
  if (audio_data->data == NULL) {
    ma_decoder_uninit(&decoder);
    return -2; // Malloc failed
  }

  // Decode the entire audio file.
  ma_decoder_read_pcm_frames(&decoder, audio_data->data, audio_data->n_frames,
                             NULL);

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
  audio_data->data =
      (float *)malloc(audio_data->n_frames * config.channels * sizeof(float));
  if (audio_data->data == NULL) {
    ma_decoder_uninit(&decoder);
    return -2; // Malloc failed
  }

  // Decode the entire audio file.
  ma_decoder_read_pcm_frames(&decoder, audio_data->data, audio_data->n_frames,
                             NULL);

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

#ifndef GM_NO_GAPI

#include "gapi.h"

/**
 * @brief Loads an audio file from a path and prepares it for playback.
 *
 * This function decodes the entire audio file into memory, sends the data
 * to the backend, and then frees the CPU-side memory.
 *
 * @param path The file path to the audio file (e.g., .wav, .ogg, .mp3).
 * @return A gmAudio object. The handle will be 0 if loading fails.
 */
static inline int gm_load_audio(gmAudio *audio, const char *path) {
  audio->handle = 0;
  gmAudioData audio_data;
  int ret;
  ret = gm_audio_data_load(&audio_data, path);
  if (ret != 0) {
    return ret;
  }

  uint32_t handle =
      gapi_create_audio(audio_data.data, audio_data.n_frames,
                        audio_data.n_channels, audio_data.sample_rate);

  // gm_audio_data_free(&audio_data);
  audio->handle = handle;

  return 0;
}

static inline int gm_load_audio_from_memory(gmAudio *audio,
                                            const unsigned char *data,
                                            size_t len) {
  audio->handle = 0;
  gmAudioData audio_data;
  int ret;
  ret = gm_audio_data_load_from_memory(&audio_data, data, len);
  if (ret != 0) {
    return ret;
  }

  uint32_t handle =
      gapi_create_audio(audio_data.data, audio_data.n_frames,
                        audio_data.n_channels, audio_data.sample_rate);

  // gm_audio_data_free(&audio_data);
  audio->handle = handle;

  return 0;
}

/**
 * @brief Plays a loaded audio file.
 *
 * @param audio The gmAudio object to play.
 * @param loop If true, the sound will play continuously until stopped.
 */
static inline int gm_play_audio(gmAudio audio, bool loop) {
  if (audio.handle > 0) {
    return gapi_play_audio(audio.handle, loop);
  } else {
    return 1;
  }
}

/**
 * @brief Stops a playing audio file.
 *
 * @param audio The gmAudio object to stop.
 */
static inline int gm_stop_audio(gmAudio audio) {
  if (audio.handle > 0) {
    return gapi_stop_audio(audio.handle);
  } else {
    return 1;
  }
}

/**
 * @brief Frees a loaded audio resource from the engine.
 *
 * Call this when you are finished with a sound to free up resources.
 *
 * @param audio The gmAudio object to free.
 */
static inline int gm_free_audio(gmAudio audio) {
  if (audio.handle > 0) {
    return gapi_free_audio(audio.handle);
  } else {
    return 1;
  }
}

#endif
#endif // GM_AUDIO_H_INCLUDED
