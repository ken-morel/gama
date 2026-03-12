/**
 * @file gapi.h
 * @brief Graphics API (GAPI) abstraction layer for Gama.
 *
 * This file declares the interface for platform-specific graphics operations.
 * It provides a set of `gapi_` prefixed functions that are implemented
 * differently for each target platform (e.g., native desktop, WebAssembly).
 * This abstraction allows the core Gama engine code to remain
 * platform-agnostic.
 */
#ifndef GM_GAPI_H_INCLUDED
#define GM_GAPI_H_INCLUDED

#ifdef GM_NO_GAPI
#error "gapi.h included but GM_NO_GAPI was defined"
#endif

#include "color.h"
#include <stdint.h>

/**
 * @brief Sets the title of the application window.
 * @param title The null-terminated string for the new window title.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("set_title")))
#endif
#endif
    gapi_set_title(const char *title);

/**
 * @brief Resizes the application window.
 * @param width The new width of the window in pixels.
 * @param height The new height of the window in pixels.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("resize")))
#endif
#endif
    gapi_resize(const int32_t width, const int32_t height);

/**
 * @brief Sets the background color of the application window.
 * @param background The `gmColor` to set as the background.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("set_background_color")))
#endif
#endif
    gapi_set_background_color(const gmColor background);

/**
 * @brief Toggles fullscreen mode for the application window.
 * @param fullscreen 1 to enable fullscreen, 0 to disable.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("fullscreen")))
#endif
#endif
    gapi_fullscreen(const int32_t fullscreen);

// --- Utils ---
/**
 * @brief Logs a message to the platform's console.
 * @param message The null-terminated string to log.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("log")))
#endif
#endif
    gapi_log(const char *message);
/**
 * @brief Logs a message to the platform's console.
 * @param message The null-terminated string to log.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("log_error")))
#endif
#endif
    gapi_log_error(const char *message);
/**
 * @brief Logs a message to the platform's console.
 * @param message The null-terminated string to log.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("log_warning")))
#endif
#endif
    gapi_log_warning(const char *message);

// --- Game Loop ---
/**
 * @brief Initializes the Graphics API and the application window.
 *
 * This function must be called once at the start of the application.
 *
 * @param width The desired width of the window.
 * @param height The desired height of the window.
 * @param title The title of the window.
 * @return 0 on success, non-zero on failure.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("init")))
#endif
#endif
    gapi_init(const int32_t width, const int32_t height, const char *title);

/**
 * @brief Yields control to the platform, processes events, and updates timing.
 *
 * This function should be called at the end of each frame. It handles
 * window events, updates input states, swaps buffers, and calculates `_gm_dt`.
 *
 * @param dt A pointer to a double where the calculated delta time will be
 * stored.
 * @return 1 if the application should continue, 0 if it should exit.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("yield")))
#endif
#endif
    gapi_yield(double *dt); // Changed to int32_t return type

/**
 * @brief Requests the application to quit.
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("quit")))
#endif
#endif
    gapi_quit();

// --- Drawing Primitives ---
/**
 * @brief Draws a line segment on the screen.
 * @param x1 The x-coordinate of the start point.
 * @param y1 The y-coordinate of the start point.
 * @param x2 The x-coordinate of the end point.
 * @param y2 The y-coordinate of the end point.
 * @param thickness The thickness of the line.
 * @param col The `gmColor` of the line.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_line")))
#endif
#endif
    gapi_draw_line(double x1, double y1, double x2, double y2, double thickness,
                   gmColor col);
/**
 * @brief Draws a filled rectangle on the screen.
 * @param x The x-coordinate of the center of the rectangle.
 * @param y The y-coordinate of the center of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param col The `gmColor` of the rectangle.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_rect")))
#endif
#endif
    gapi_draw_rect(double x, double y, double w, double h, gmColor col);

/**
 * @brief Draws a filled rectangle with rounded corners on the screen.
 * @param x The x-coordinate of the center of the rectangle.
 * @param y The y-coordinate of the center of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param r The corner radius.
 * @param col The `gmColor` of the rectangle.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_rounded_rect")))
#endif
#endif
    gapi_draw_rounded_rect(double x, double y, double w, double h, double r,
                           gmColor col);

/**
 * @brief Draws a filled circle on the screen.
 * @param center_x The x-coordinate of the center of the circle.
 * @param center_y The y-coordinate of the center of the circle.
 * @param radius The radius of the circle.
 * @param col The `gmColor` of the circle.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_circle")))
#endif
#endif
    gapi_draw_circle(double center_x, double center_y, double radius,
                     gmColor col);

/**
 * @brief Draws a filled ellipse on the screen.
 * @param x The x-coordinate of the center of the ellipse.
 * @param y The y-coordinate of the center of the ellipse.
 * @param w The width of the ellipse.
 * @param h The height of the ellipse.
 * @param col The `gmColor` of the ellipse.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_ellipse")))
#endif
#endif
    gapi_draw_ellipse(double x, double y, double w, double h, gmColor col);

/**
 * @brief Draws a filled triangle on the screen.
 * @param x1 The x-coordinate of the first vertex.
 * @param y1 The y-coordinate of the first vertex.
 * @param x2 The x-coordinate of the second vertex.
 * @param y2 The y-coordinate of the second vertex.
 * @param x3 The x-coordinate of the third vertex.
 * @param y3 The y-coordinate of the third vertex.
 * @param col The `gmColor` of the triangle.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_triangle")))
#endif
#endif
    gapi_draw_triangle(double x1, double y1, double x2, double y2, double x3,
                       double y3, gmColor col);

/**
 * @brief Draws a batch of triangles on the screen.
 * @param n_triangles The number of triangles in the batch.
 * @param points An array of `double`s representing the vertices (x1,y1, x2,y2,
 * x3,y3 for each triangle).
 * @param colors An array of `gmColor`s, one for each triangle.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_triangles")))
#endif
#endif
    gapi_draw_triangles(uint32_t n_triangles, double *points, gmColor *colors);

/**
 * @brief Creates a platform-specific image handle from raw pixel data.
 *
 * This function is used to upload image data to the GPU or create a drawable
 * surface that can be used by `gapi_draw_image` functions.
 *
 * @param data A pointer to the raw RGBA pixel data.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return A unique handle (ID) for the created image on success, 0 on failure.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("create_image")))
#endif
#endif
    gapi_create_image(const unsigned char *data, uint32_t width,
                      uint32_t height);

/**
 * @brief Draws an image referenced by its handle on the screen.
 * @param handle The handle of the image to draw, obtained from
 * `gapi_create_image`.
 * @param x The x-coordinate of the center of the image.
 * @param y The y-coordinate of the center of the image.
 * @param width The width to draw the image.
 * @param height The height to draw the image.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_image")))
#endif
#endif
    gapi_draw_image(int32_t handle, double x, double y, double width,
                    double height);
/**
 * @brief Draws a part of an image referenced by its handle on the screen.
 * @param handle The handle of the image to draw from.
 * @param slice_x The x-coordinate of the top-left corner of the source slice.
 * @param slice_y The y-coordinate of the top-left corner of the source slice.
 * @param slice_width The width of the source slice.
 * @param slice_height The height of the source slice.
 * @param x The x-coordinate of the center of the destination rectangle.
 * @param y The y-coordinate of the center of the destination rectangle.
 * @param width The width to draw the slice.
 * @param height The height to draw the slice.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_image_part")))
#endif
#endif
    gapi_draw_image_part(int32_t handle, uint32_t slice_x, uint32_t slice_y,
                         uint32_t slice_width, uint32_t slice_height, double x,
                         double y, double width, double height);

// --- Text Functions ---
/**
 * @brief Draws text on the screen.
 * @param x The x-coordinate of the center of the text.
 * @param y The y-coordinate of the center of the text.
 * @param height The height/size of the text.
 * @param txt The null-terminated string to draw.
 * @param font The null-terminated font name to use.
 * @param style A bitmask for text style (e.g., bold, italic).
 * @param col The `gmColor` of the text.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("draw_text")))
#endif
#endif
    gapi_draw_text(double x, double y, double height, const char *txt,
                   const char *font, uint8_t style, gmColor col);

// --- Event Functions ---
/**
 * @brief Checks if a specific key is currently pressed.
 * @param t The type of key ('c' for character, 'a' for arrow, 's' for special).
 * @param k The key code.
 * @return 1 if the key is down, 0 otherwise.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("key_down")))
#endif
#endif
    gapi_key_down(char t, char k);

/**
 * @brief Checks if the mouse button is currently pressed.
 * @return 1 if the mouse button is down, 0 otherwise.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("mouse_down")))
#endif
#endif
    gapi_mouse_down();

/**
 * @brief Retrieves the current mouse cursor position.
 * @param x A pointer to a double to store the x-coordinate.
 * @param y A pointer to a double to store the y-coordinate.
 * @return 0 on success.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("mouse_get")))
#endif
#endif
    gapi_mouse_get(double *x, double *y);

#ifndef GM_NATIVE
// --- Audio Functions ---

/**
 * @brief Creates a platform-specific audio resource from raw PCM data.
 *
 * This function is used to upload decoded audio data to the audio backend
 * where it can be played by `gapi_audio_play`.
 *
 * @param data A pointer to the decoded float audio data.
 * @param frame_count The total number of PCM frames.
 * @param channels The number of audio channels (e.g., 1 for mono, 2 for
 * stereo).
 * @param sample_rate The sample rate of the audio (e.g., 48000).
 * @return A unique handle (ID) for the created audio resource on success, 0 on
 * failure.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("create_audio")))
#endif
#endif
    gapi_create_audio(const float *data, uint64_t frame_count,
                      uint32_t channels, uint32_t sample_rate);

/**
 * @brief Plays the audio resource associated with the handle.
 *
 * @param handle The handle of the audio resource to play.
 * @param loop Whether the audio should loop continuously (1 for true, 0 for
 * false).
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("play_audio")))
#endif
#endif
    gapi_play_audio(uint32_t handle, int32_t loop);

/**
 * @brief Stops the audio resource associated with the handle.
 *
 * @param handle The handle of the audio resource to stop.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("stop_audio")))
#endif
#endif
    gapi_stop_audio(uint32_t handle);

/**
 * @brief Frees the audio resource in the backend.
 *
 * @param handle The handle of the audio resource to free.
 */
extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("free_audio")))
#endif
#endif
    gapi_free_audio(uint32_t handle);
#endif // GM_NATIVE

extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute((import_module("gapi"), import_name("snap")))
#endif
#endif
    gapi_snap(int32_t handle);

extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute((import_module("gapi"), import_name("get_size")))
#endif
#endif
    gapi_get_size(uint32_t *width, uint32_t *height);

extern int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute((import_module("gapi"), import_name("clear")))
#endif
#endif
    gapi_clear();

extern void // wait for all drawing operations in the queue to finish
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute((import_module("gapi"), import_name("sync")))
#endif
#endif
    gapi_sync();

/**
 * @brief Blits a pixel buffer to the screen.
 * @param data Pointer to the raw RGB pixel data (3 bytes per pixel).
 * @param width Width of the pixel buffer.
 * @param height Height of the pixel buffer.
 * @param x Center X coordinate for the blit (centered).
 * @param y Center Y coordinate for the blit (centered).
 * @param target_width Target width to draw (0 for natural size).
 * @param target_height Target height to draw (0 for natural size).
 */
extern void
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((import_module("gapi"), import_name("blit")))
#endif
#endif
    gapi_blit(const uint8_t *data, int32_t width, int32_t height, double x,
              double y, double target_width, double target_height);

#endif // GM_GAPI_H_INCLUDED
