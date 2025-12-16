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
#include <stdio.h>

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

/**
 * @brief Checks if the main game loop should continue running.
 * @return 1 if the window is open and the game should continue, 0 otherwise.
 * @deprecated This function is deprecated and will be removed. Use gm_yield()
 * instead.
 */
static inline int gm_runs() { return gapi_runs(); }

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
void gm_sync() { gapi_wait_queue(); }

/**
 * @brief Sets the background color of the window.
 * @param c The color to set as the background.
 */
void gm_bg_color(gmColor c) {
  gapi_set_bg_color(gm_red(c), gm_green(c), gm_blue(c), gm_alpha(c));
}
