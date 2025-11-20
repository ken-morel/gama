#ifndef GAMA_H_INCLUDED

//NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 0

#include <time.h>
#include "gapi.h"
#include "color.h"
#include "shape.h"


int GAMA_RUNS = 0;
double gama_last_update_time;
double gama_get_time_secs(){
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000;
    } else {
      return 0.01;
    }
}
void gama_init() {
  GAMA_RUNS = 1;
  if(!gapi_init()) {
    return;
  } else {
    gapi_log("Error starting gama, initialization exited with non zero code");
  }
  gama_last_update_time = gama_get_time_secs();
}
int gama_runs() {
  return GAMA_RUNS;
}
double  gama_yield() {
  double ctime = gama_get_time_secs();
  double dt = ctime - gama_last_update_time;
  gama_last_update_time = ctime;
  return dt;
}

void gama_quit() {
  GAMA_RUNS = 0;
  gapi_quit();
}

#define GAMA_H_INCLUDED
#endif
