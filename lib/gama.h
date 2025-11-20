#ifndef GAMA_H_INCLUDED

// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 0

#include "gapi.h"
#include "shape.h"
#include <stdio.h>


void gama_init() {
  int code = gapi_init();
  char msg[100];

  if (code != 0) {
    sprintf(msg,
            "Error starting gama, initialization exited with non zero code %d",
            code);
    gapi_log(msg);
  }
}
int gama_runs() { return gapi_runs(); }
double gama_yield() { return gapi_yield(); }

void gama_quit() { gapi_quit(); }

#define GAMA_H_INCLUDED
#endif
