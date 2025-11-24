#pragma once


#define CP_USE_DOUBLES 1
// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 0

#include "gapi.h"
#include "body.h"
#include "system.h"
#include "physics.h"
#include "draw.h"
#include "key.h"
#include <stdio.h>

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
static inline int gm_runs() { return gapi_runs(); }
static inline int gm_yield() {
  if (gapi_yield(&_gm_dt)) {
    gapi_get_mouse_move(&gm_mouse.movement.x, &gm_mouse.movement.y);
    gapi_mouse_get(&gm_mouse.position.x, &gm_mouse.position.y);
    gm_mouse.down = gapi_mouse_down();
    gm_mouse.pressed = gapi_mouse_pressed();
    return 1;
  } else
    return 0;
}
static inline void gm_quit() { return gapi_quit(); }

void gm_sync() { gapi_wait_queue(); }
