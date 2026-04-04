/**
 * @file gama.h
 * @brief Core header file for the Gama engine.
 *
 * This file includes all the necessary headers and provides the main
 * functions for initializing the engine, managing the game loop, and handling
 * basic window operations.
 */

#ifndef GM_GAMA_H_INCLUDED
#define GM_GAMA_H_INCLUDED
#include "color.h"
#include "draw.h"
#include "gapi.h"
#include "renderer.h"
#include "snap.h"
#include "stdio.h"
#include "t.h"
#include "widgets/frame.h"
#include "window.h"
#ifdef GM_NATIVE
#include "sound.h"
#endif

#ifdef GM_BUILTIN_RENDERER
gmScreen _gm_screen;
#endif

#ifdef GM_ARGC_MAIN
int main(int, char **);
#else
int main();
#endif

/**
 * @internal
 * @brief Main entry point for the Gama application, called by the platform
 * runner. This function calls the user-defined main().
 */
int32_t
#ifdef GM_ZCC
#ifdef GM_WEB
    __attribute__((export_name("gama_run")))
#endif
#endif
    gama_run() {
#ifdef GM_ARGC_MAIN
  return main(0, NULL);
#else
  return main();
#endif
}

/**
 * @brief Puts the window in fullscreen.
 *
 * @param fullscreen Boolean flag to enable (1) or disable (0) fullscreen.
 */
void gm_fullscreen(int fullscreen) { return gapi_fullscreen(fullscreen); }

/**
 * @brief Draws the Gama logo.
 *
 * @param x The center x position to draw the logo.
 * @param y The center y position to draw the logo.
 * @param s The size (side length) of the logo.
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
/**
 * @brief Enables or disables the built-in FPS counter display.
 * @param show Boolean flag to show (1) or hide (0) the FPS counter.
 */
int __gm_show_fps = 0;
void gm_show_fps(int show) { __gm_show_fps = show; }

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
  static const double alpha = 2.0 / 3.0;
  static double _fps = 0;
  static double dt = 1;
  static double _display_fps = 0;
  double current_dt = gm_dt();
  dt += current_dt;
  double fps = 1 / current_dt;
  if (current_dt == 0)
    fps = _fps;
  if (_fps == 0)
    _fps = 60;
  else
    _fps = (_fps * alpha) + (fps * (1 - alpha));
  if (dt >= 0.5) {
    dt = 0;
    _display_fps = _fps;
  }

  gmWindow.prevHeight = gmWindow.height;
  gmWindow.prevWidth = gmWindow.width;
  gapi_get_size(&gmWindow.width, &gmWindow.height);
  gmWindow.resized = gmWindow.prevHeight != gmWindow.height ||
                     gmWindow.prevWidth != gmWindow.width;
  if (gmWindow.resized)
    gm_unsnap(0);

  if (__gm_show_fps) {
    char fps_text[20] = {0}; // ERROR: use fps
    snprintf(fps_text, sizeof(fps_text), "fps: %.2f", _display_fps);
    gmw_frame(0.9, -0.9, 0.4, 0.1);
    gm_draw_text(0.9, -0.9, fps_text, "", 0.1, GM_WHITE);
  }

#ifdef GM_BUILTIN_RENDERER
  gapi_blit(_gm_screen.data, _gm_screen.width, _gm_screen.height, 0, 0, 0, 0);
#endif

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

  return ret;
}

/**
 * @brief Closes the window and terminates the Gama engine.
 */
static inline void gm_quit() {
  gm_clear();
  gm_draw_text(0, 0, "bye", "", 0.3, GM_GAMA);
  return gapi_quit();
}

/**
 * @brief Sets the background color of the window.
 * @param c The color to set as the background.
 */
void gm_background(gmColor c) {
  gmWindow.background = c;
  gapi_set_background_color(c);
}

/**
 * @brief Resizes the application window.
 * @param width The new width of the window in pixels.
 * @param height The new height of the window in pixels.
 */
void gm_resize(int width, int height) { return gapi_resize(width, height); }

/**
 * @brief Initializes the Gama engine and opens a window.
 *
 * This must be the first Gama function called. It sets up the graphics context
 * and creates a window with the specified dimensions and title.
 *
 * @param width The width of the window in pixels. Use 0 for automatic sizing.
 * @param height The height of the window in pixels. Use 0 for automatic sizing.
 * @param title The title of the window.
 */
void gm_init(int width, int height, const char *title) {

  int code = gapi_init(width, height, title);
  if (code != 0) {
    char msg[100];
    snprintf(msg, sizeof(msg),
             "Error starting gama, initialization exited with non zero code %d",
             code);
    gapi_log_error(msg);
  }

#ifdef GM_BUILTIN_RENDERER
  uint32_t w, h;
  gapi_get_size(&w, &h);
  _gm_screen.width = w;
  _gm_screen.height = h;
  _gm_screen.data = (uint8_t *)malloc(w * h * 3);
#endif

#ifdef GM_NATIVE
  gm_audio_init();
#endif
  gmWindow.width = width;
  gmWindow.height = height;
  gmWindow.prevHeight = height;
  gmWindow.prevWidth = width;
  gmWindow.background = GM_BLACK;
}

void gm_loading() {
  gm_background(GM_BLACK);
  for (size_t i = 0; i < 5; i++) {
    gm_logo(0, 0, 1);
    gm_draw_text(0, -0.8, "Loading...", "", 0.2, GM_GAMA);
    gm_yield();
  }
}

/**
 * @brief Pauses execution for a specified duration.
 *
 * This function blocks the calling thread for approximately the given number
 * of milliseconds.
 *
 * @param milliseconds The number of milliseconds to sleep.
 */
void gm_sleep(int milliseconds);

#ifdef GM_ZCC
void gm_sleep(int m) {};
#else
#ifdef _WIN32
#include <windows.h>
void gm_sleep(int milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
void gm_sleep(int milliseconds) { usleep(milliseconds * 1000); }
#endif
#endif

#endif // GM_GAMA_H_INCLUDED
