#pragma once

#include "gapi.h"
#include <stdint.h>

typedef struct {
  uint32_t handle;
  uint32_t width;
  uint32_t height;
} gmImage;

gmImage gm_image_open(const char *path) {
  gmImage img;
  img.handle = gapi_create_image(path, &img.width, &img.height);
  return img;
}

void gm_image_draw(gmImage i, double x, double y, double w, double h) {
  gapi_draw_image(i.handle, x, y, w, h);
}
