/**
 * @file gama.h
 * @brief Core header file for the Gama engine.
 *
 * This file includes all the necessary headers and provides the main
 * functions for initializing the engine, managing the game loop, and handling
 * basic window operations.
 */

#pragma once
// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 0

#include "draw.h"
#include "gapi.h"
#include "stdio.h"
#include "widgets/frame.h"

int _gm_loop();

#ifdef GM_SETUP

int32_t
#ifdef __ZIG_CC__
    __attribute__((export_name("gama_mode")))
#endif
    gama_mode() {
  return 2;
}

int setup();
int loop();

__attribute__((export_name("gama_setup"))) int32_t gama_setup() {
  return setup();
  // ama
}
__attribute__((export_name("gama_loop"))) int32_t gama_loop() {
  if (_gm_loop()) {
    return loop();
  } else
    return 0;
}

#ifdef GM_NATIVE

#include <stdio.h>

int main(void) {
  int code = setup();
  if (code != 0)
    return code;
  while (_gm_loop()) {
    code = loop();
    if (code != 0)
      return code;
  }
  return 0;
}

#endif

#else
int main(void);

__attribute__((export_name("gama_mode"))) int32_t gama_mode() { return 1; }

__attribute__((export_name("gama_run"))) int32_t gama_run() { return main(); }

#ifdef GM_WEB

#warning "To build for the web, your app must be in GM_SETUP mode"

#endif

#endif

/**
 * @brief Puts the window in fullscreen.
 *
 * @param fullscreen Enable or disable fullscreen.
 */
void gm_fullscreen(int fullscreen) { return gapi_fullscreen(fullscreen); }

/**
 * @brief Draws gama logo.
 *
 * @param x The x position to draw the logo.
 * @param y The y position to draw the logo.
 * @param s The logo size.
 */
void gm_logo(double x, double y, double s) {
  double top_thickness = 0.15 * s;
  double left_thickness = 0.1 * s;
  double ratio = 0.6;
  // top bar
  gm_draw_rectangle(x, y + s / 2 - top_thickness / 2, s * ratio, top_thickness,
                    GM_GAMA);
  gm_draw_rectangle(x + (-s / 2) + (left_thickness / 2) + (s / 2 * (1 - ratio)),
                    y, left_thickness, s, GM_GAMA);
}

void gm_log(const char *txt) { return gapi_log(txt); }

/**
 * @brief Checks if the main game loop should continue running.
 * @return 1 if the window is open and the game should continue, 0 otherwise.
 * @deprecated This function is deprecated and will be removed. Use gm_yield()
 * instead.
 */
static inline int gm_runs() { return gapi_runs(); }

int __gm_show_fps = 0;
void gm_show_fps(int show) { __gm_show_fps = show; }

void _gm_fps() {
  static const double alpha = 0.9;
  static double _fps = 0;
  static double dt = 1;
  static double _display_fps = 0;
  dt += gm_dt();
  double fps = 1 / gm_dt();
  if (_fps == 0)
    _fps = 60;
  else
    _fps = (_fps * alpha) + (fps * (1 - alpha));
  if (dt >= 0.5) {
    dt = 0;
    _display_fps = _fps;
  }

  if (__gm_show_fps) {
    char fps_text[20] = {0}; // ERROR: use fps
    // snprintf(fps_text, sizeof(fps_text), "fps: %.2lf", _display_fps);
    snprintf(fps_text, sizeof(fps_text), "fps: %d", (int)_display_fps);
    gmw_frame(0.9, -0.9, 0.4, 0.1);
    gm_draw_text(0.9, -0.9, fps_text, "", 0.1, GM_WHITE);
  }
}
#ifndef GM_SETUP

/**
 * @brief Processes events, updates input state, and prepares for the next
 * frame.
 *
 * This function should be called at the end of the main game loop. It handles
 * window events, polls for input, updates mouse and keyboard states, and
 * swaps the graphics buffers.
 *
 * @return 1 if the game should continue to the next frame, 0 if the window has
 * been closed.
 * @example
 * while (gm_yield()) {
 *   // Your game logic and rendering here
 * }
 */
static inline int gm_yield() { return _gm_loop(); }
#endif

int _gm_loop() {
  const int ret = gapi_yield(&_gm_dt);
  _gm_t += _gm_dt;
  gm_mouse.lastPosition = gm_mouse.position;
  gapi_mouse_get(&gm_mouse.position.x, &gm_mouse.position.y);
  gm_mouse.movement.x = gm_mouse.position.x - gm_mouse.lastPosition.x;
  gm_mouse.movement.y = gm_mouse.position.y - gm_mouse.lastPosition.y;
  gm_mouse.down = gapi_mouse_down();
  static int last_mouse_down = 0;
  gm_mouse.clicked = !last_mouse_down && gm_mouse.down;
  last_mouse_down = gm_mouse.down;
  _gm_fps();
  return ret;
}

/**
 * @brief Closes the window and terminates the Gama engine.
 */
static inline void gm_quit() { return gapi_quit(); }

/**
 * @brief Waits for all pending graphics operations to complete.
 *
 * This can be used for synchronization purposes, but it is rarely needed as
 * gm_yield() handles buffer swapping automatically.
 */
void gm_sync() { return gapi_wait_queue(); }

/**
 * @brief Sets the background color of the window.
 * @param c The color to set as the background.
 */
void gm_background(gmColor c) {
  return gapi_set_bg_color(gm_red(c), gm_green(c), gm_blue(c), gm_alpha(c));
}

void gm_resize(int width, int height) { return gapi_resize(width, height); }

/**
 * @brief Initializes the Gama engine and opens a window.
 *
 * This must be the first Gama function called. It sets up the graphics context
 * and creates a window with the specified dimensions and title.
 *
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param title The title of the window.
 */
void gm_init(int width, int height, const char *title) {
  int code = gapi_init(width, height, title);
  char msg[100];

  if (code != 0) {
    snprintf(msg, sizeof(msg),
             "Error starting gama, initialization exited with non zero code %d",
             code);
    gapi_log(msg);
  }
  gm_background(GM_BLACK);
}

/**
 * @brief Sleep(wait) for aproximately the specified number of milliseconds
 *
 * @param miliseconds the number of milliseconds to sleep
 */
void gm_sleep(int milliseconds);

#ifdef __ZIG_CC__
void gm_sleep(int m) {};
#else
#ifdef _WIN32
#include <windows.h>
void gm_sleep(int milliseconds) { Sleep(milliseconds); }
#include <unistd.h>
void gm_sleep(int milliseconds) { usleep(milliseconds * 1000); }
#endif
#endif
