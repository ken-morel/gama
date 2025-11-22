#pragma once

#include "gapi.h"
#include <stdint.h>

typedef struct {
  uint32_t handle;
  uint32_t width;
  uint32_t height;
} gama_image;

gama_image gama_image_open(const char *path) {
  gama_image img;
  img.handle = gapi_create_image(path, &img.width, &img.height);
  return img;
}


void gama_image_draw(gama_image i, double x, double y, double w, double h) {
  gapi_draw_image(i.handle, x, y, w, h);
}

