#ifndef GM_AUDIO_H_INCLUDED
#define GM_AUDIO_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>  // For printf debugging
#include <stdlib.h> // For malloc and free

// ==============================================================================
// NATIVE AUDIO IMPLEMENTATION (when GM_NATIVE is defined)
// ==============================================================================
#ifdef GM_NATIVE

#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
#define MA_ENABLE_PULSEAUDIO
#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio.h"

// Global audio engine, managed by this header
static ma_engine g_gm_audio_engine;
static bool g_gm_audio_engine_initialized = 0;

/**
 * @brief Initializes the native audio engine. Must be called once at startup.
 * @return 0 on success, non-zero on failure.
 */
static inline int gm_audio_init() {
  if (g_gm_audio_engine_initialized) {
    printf("[GMAUDIO] Audio engine already initialized.\n");
    return 0;
  }
  ma_result result = ma_engine_init(NULL, &g_gm_audio_engine);
  if (result != MA_SUCCESS) {
    printf("[GMAUDIO:ERROR] Failed to initialize audio engine. Code: %d\n",
           result);
    return -1;
  }
  printf("[GMAUDIO] Audio engine initialized successfully.\n");

  // DEBUG: Check and set master volume.
  float initial_volume = ma_engine_get_volume(&g_gm_audio_engine);
  printf("[GMAUDIO:DEBUG] Initial master volume: %f\n", initial_volume);
  ma_engine_set_volume(&g_gm_audio_engine, 1.0f);
  printf("[GMAUDIO:DEBUG] Master volume explicitly set to 1.0\n");

  g_gm_audio_engine_initialized = 1;
  return 0;
}

/**
 * @brief Uninitializes the native audio engine. Must be called once at
 * shutdown.
 */
static inline void gm_audio_uninit() {
  if (g_gm_audio_engine_initialized) {
    ma_engine_uninit(&g_gm_audio_engine);
    g_gm_audio_engine_initialized = 0;
    printf("[GMAUDIO] Audio engine uninitialized.\n");
  }
}

/**
 * @brief Represents a playable sound. For native builds, this is a wrapper
 *        around miniaudio objects.
 */
typedef struct {
  ma_sound sound;
  ma_decoder *pDecoder; // Required for sounds loaded from memory
  bool initialized;
} gmSound;

/**
 * @brief Loads an audio file from a path and prepares it for playback.
 * @param audio Pointer to the gmSound object to initialize.
 * @param path The file path to the audio file (e.g., .wav, .ogg, .mp3).
 * @return 0 on success, non-zero on failure.
 */
static inline int gm_load_sound(gmSound *audio, const char *path) {
  if (!audio) {
    printf("[GMAUDIO:ERROR] gm_load_sound: audio pointer is NULL.\n");
    return -1;
  }
  if (!g_gm_audio_engine_initialized) {
    printf("[GMAUDIO:ERROR] gm_load_sound: audio engine not initialized.\n");
    return -1;
  }
  audio->initialized = 0;
  audio->pDecoder = NULL; // Not used when loading from file

  printf("[GMAUDIO] Loading sound from file: %s\n", path);
  ma_result result = ma_sound_init_from_file(&g_gm_audio_engine, path, 0, NULL,
                                             NULL, &audio->sound);
  if (result != MA_SUCCESS) {
    printf("[GMAUDIO:ERROR] Failed to load sound from file. Code: %d\n",
           result);
    return -2;
  }
  audio->initialized = 1;
  printf("[GMAUDIO] Sound loaded successfully.\n");
  return 0;
}

/**
 * @brief Loads an audio file from an in-memory buffer and prepares it for
 * playback.
 * @param audio Pointer to the gmSound object to initialize.
 * @param data Pointer to the file data in memory (e.g., contents of a .wav
 * file).
 * @param len The length of the data in bytes.
 * @return 0 on success, non-zero on failure.
 */
static inline int gm_load_sound_from_memory(gmSound *audio,
                                            const unsigned char *data,
                                            size_t len) {
  if (!audio || !g_gm_audio_engine_initialized)
    return -1;
  audio->initialized = 0;

  audio->pDecoder = (ma_decoder *)malloc(sizeof(ma_decoder));
  if (audio->pDecoder == NULL) {
    printf("[GMAUDIO:ERROR] Failed to allocate memory for decoder.\n");
    return -2;
  }

  printf("[GMAUDIO] Loading sound from memory (%zu bytes)\n", len);
  ma_result result = ma_decoder_init_memory(data, len, NULL, audio->pDecoder);
  if (result != MA_SUCCESS) {
    printf("[GMAUDIO:ERROR] Failed to init decoder from memory. Code: %d\n",
           result);
    free(audio->pDecoder);
    audio->pDecoder = NULL;
    return -3;
  }

  result = ma_sound_init_from_data_source(&g_gm_audio_engine, audio->pDecoder,
                                          0, NULL, &audio->sound);
  if (result != MA_SUCCESS) {
    printf("[GMAUDIO:ERROR] Failed to init sound from data source. Code: %d\n",
           result);
    ma_decoder_uninit(audio->pDecoder);
    free(audio->pDecoder);
    audio->pDecoder = NULL;
    return -4;
  }

  audio->initialized = 1;
  printf("[GMAUDIO] Sound loaded from memory successfully.\n");
  return 0;
}

/**
 * @brief Frees a loaded audio resource.
 * @param audio The gmSound object to free.
 * @return 0 on success.
 */
static inline int gm_free_sound(gmSound audio) {
  if (audio.initialized) {
    printf("[GMAUDIO] Freeing sound.\n");
    ma_sound_uninit(&audio.sound);
    // If it was loaded from memory, we also need to free the decoder.
    if (audio.pDecoder != NULL) {
      ma_decoder_uninit(audio.pDecoder);
      free(audio.pDecoder);
    }
    return 0;
  }
  return 1;
}

/**
 * @brief Plays a loaded audio file.
 * @param audio The gmSound object to play.
 * @param loop If 1, the sound will play continuously until stopped.
 * @return 0 on success.
 */
static inline int gm_play_sound(gmSound audio, bool loop) {
  if (audio.initialized) {
    printf("[GMAUDIO] Playing sound (loop: %d)...\n", loop);
    ma_sound_set_looping(&audio.sound, loop);
    ma_result result = ma_sound_start(&audio.sound);
    if (result != MA_SUCCESS) {
      printf("[GMAUDIO:ERROR] Failed to start sound. Code: %d\n", result);
      return 1;
    }
    // Check state immediately after starting
    bool isPlaying = ma_sound_is_playing(&audio.sound);
    bool isAtEnd = ma_sound_at_end(&audio.sound);
    printf("[GMAUDIO] Sound state after start: isPlaying=%d, isAtEnd=%d\n",
           isPlaying, isAtEnd);
    return 0;
  } else {
    printf("[GMAUDIO:WARN] Attempted to play uninitialized sound.\n");
    return 1;
  }
}

/**
 * @brief Stops a playing audio file.
 * @param audio The gmSound object to stop.
 * @return 0 on success.
 */
static inline int gm_stop_sound(gmSound audio) {
  if (audio.initialized) {
    printf("[GMAUDIO] Stopping sound.\n");
    ma_sound_stop(&audio.sound);
    return 0;
  }
  return 1;
}

// ==============================================================================
// WEB (GAPI) AUDIO IMPLEMENTATION
// ==============================================================================
#else // GM_NATIVE is not defined, so we use GAPI for web

#include "gapi.h"

// For web, gmSound is just an opaque handle to a resource in the JS backend.
typedef struct {
  uint32_t handle;
} gmSound;

// The decoding part still happens in C, even for web builds.
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_DEVICE_IO
#define MA_NO_ENGINE
#include "../miniaudio.h"

typedef struct {
  ma_float *data;
  ma_uint64 n_frames;
  ma_uint32 n_channels;
  ma_uint32 sample_rate;
} gmAudioData;

static inline int gm_audio_data_load(gmAudioData *audio_data,
                                     const char *path) {
  ma_decoder decoder;
  ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
  if (ma_decoder_init_file(path, &config, &decoder) != MA_SUCCESS)
    return -1;
  ma_decoder_get_length_in_pcm_frames(&decoder, &audio_data->n_frames);
  audio_data->data = (ma_float *)malloc(
      audio_data->n_frames * decoder.outputChannels * sizeof(ma_float));
  if (audio_data->data == NULL) {
    ma_decoder_uninit(&decoder);
    return -2;
  }
  ma_decoder_read_pcm_frames(&decoder, audio_data->data, audio_data->n_frames,
                             NULL);
  audio_data->n_channels = decoder.outputChannels;
  audio_data->sample_rate = decoder.outputSampleRate;
  ma_decoder_uninit(&decoder);
  return 0;
}

static inline int gm_load_sound(gmSound *audio, const char *path) {
  audio->handle = 0;
  gmAudioData audio_data;
  if (gm_audio_data_load(&audio_data, path) != 0) {
    return -1;
  }
  audio->handle =
      gapi_create_audio(audio_data.data, audio_data.n_frames,
                        audio_data.n_channels, audio_data.sample_rate);
  free(audio_data.data);
  return audio->handle == 0 ? -1 : 0;
}

// NOTE: gm_load_sound_from_memory for web would be similar, decoding and then
// passing raw data to gapi_create_audio. This is left unimplemented for now as
// the native path is the focus.
static inline int gm_load_sound_from_memory(gmSound *audio,
                                            const unsigned char *data,
                                            size_t len) {
  audio->handle = 0;
  return -99; // Not implemented for web yet
}

static inline int gm_play_sound(gmSound audio, bool loop) {
  return audio.handle > 0 ? gapi_play_audio(audio.handle, loop) : 1;
}

static inline int gm_stop_sound(gmSound audio) {
  return audio.handle > 0 ? gapi_stop_audio(audio.handle) : 1;
}

static inline int gm_free_sound(gmSound audio) {
  return audio.handle > 0 ? gapi_free_audio(audio.handle) : 1;
}

#endif // End of GM_NATIVE vs GAPI split

#endif // GM_AUDIO_H_INCLUDED
