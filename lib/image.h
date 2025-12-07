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

void gm_image_draw_part(gmImage i, int slice_x, int slice_y, int slice_width,
                        int slice_height, double x, double y, double w,
                        double h) {
  gapi_draw_image_part(i.handle, slice_x, slice_y, slice_width, slice_height, x,
                       y, w, h);
}
