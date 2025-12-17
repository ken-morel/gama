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

#include "animate.h"
#include "body.h"
#include "draw.h"
#include "gapi.h"
#include "key.h"
#include "physics.h"
#include "sprite.h"
#include "system.h"
#include "widgets.h"
#include <stdio.h>

void gm_fullscreen(int fullscreen) { return gapi_fullscreen(fullscreen); }

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
    sprintf(msg,
            "Error starting gama, initialization exited with non zero code %d",
            code);
    gapi_log(msg);
  }
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
    char fps_text[20];
    sprintf(fps_text, "fps: %.2lf", _display_fps);
    gmw_frame(0.9, -0.9, 0.4, 0.1);
    gm_draw_text(0.9, -0.9, fps_text, "", 0.1, GM_WHITE);
  }
}

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
static inline int gm_yield() {
  if (gapi_yield(&_gm_dt)) {
    _gm_t += _gm_dt;
    gapi_get_mouse_move(&gm_mouse.movement.x, &gm_mouse.movement.y);
    gapi_mouse_get(&gm_mouse.position.x, &gm_mouse.position.y);
    gm_mouse.down = gapi_mouse_down();
    gm_mouse.pressed = gapi_mouse_pressed();
    _gm_fps();
    return 1;
  } else
    return 0;
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
