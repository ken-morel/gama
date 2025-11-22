#pragma once

// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 0

#include "color.h"
#include "draw.h" // Include draw.h for drawing functions
#include "gapi.h"
#include "image.h"
#include "key.h"
#include "physics.h" // Include physics.h for gama_body and physics functions
#include <stdio.h>

void gama_init(int width, int height, const char *title) {
  int code = gapi_init(width, height, title);
  char msg[100];

  if (code != 0) {
    sprintf(msg,
            "Error starting gama, initialization exited with non zero code %d",
            code);
    gapi_log(msg);
  }
}
static inline int gama_runs() { return gapi_runs(); }
static inline int gama_yield() { return gapi_yield(&gama_dt); }
static inline void gama_quit() { return gapi_quit(); }
