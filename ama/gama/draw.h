#pragma once

#include "color.h"
#include "gapi.h"
#include <stdint.h>

int32_t gama_draw_rectangle(double x, double y, double w, double h, Color c) {
  return gapi_draw_rect(x, y, w, h, gama_color_red(c), gama_color_green(c),
                        gama_color_blue(c), gama_color_alpha(c));
}
int32_t gama_draw_rounded_rectantle(double x, double y, double w, double h,
                                double r, Color c) {
  return gapi_draw_rounded_rect(x, y, w, h, r, gama_color_red(c),
                                gama_color_green(c), gama_color_blue(c),
                                gama_color_alpha(c));
}

int32_t gama_draw_circle(double center_x, double center_y, double radius, Color c) {
  return gapi_draw_circle(center_x, center_y, radius, gama_color_red(c),
                          gama_color_green(c), gama_color_blue(c),
                          gama_color_alpha(c));
}

int32_t gama_draw_ellipse(double x, double y, double w, double h, Color c) {
  return gapi_draw_ellipse(x, y, w, h, gama_color_red(c), gama_color_green(c),
                           gama_color_blue(c), gama_color_alpha(c));
}

int32_t gama_draw_triangle(double x1, double y1, double x2, double y2, double x3,
                       double y3, Color c) {
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, gama_color_red(c),
                            gama_color_green(c), gama_color_blue(c),
                            gama_color_alpha(c));
}

