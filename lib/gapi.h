#pragma once
#include <stdint.h>

// canvas

extern void gapi_set_title(const char *title);

extern void gapi_resize(int32_t width, int32_t height);
extern void gapi_set_bg_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

extern void gapi_fullscreen(int32_t fullscreen);

// utils
extern void gapi_log(const char *message);

// game loop
extern int32_t gapi_init(int32_t width, int32_t height, const char *title);
extern int gapi_yield(double *theta);
extern void gapi_quit();
extern int32_t gapi_runs();

// draw functions

extern int32_t gapi_draw_rect(double x, double y, double w, double h,
                              uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca);

extern int32_t gapi_draw_rounded_rect(double x, double y, double w, double h,
                                      double r, uint8_t cr, uint8_t cg,
                                      uint8_t cb, uint8_t ca);

extern int32_t gapi_draw_circle(double center_x, double center_y, double radius,
                                uint8_t red, uint8_t green, uint8_t blue,
                                uint8_t alpha);
extern int32_t gapi_draw_ellipse(double x, double y, double w, double h,
                                 uint8_t cr, uint8_t cg, uint8_t cb,
                                 uint8_t ca);

extern int32_t gapi_draw_triangle(double x1, double y1, double x2, double y2,
                                  double x3, double y3, uint8_t cr, uint8_t cg,
                                  uint8_t cb, uint8_t ca);

// image
extern uint32_t gapi_create_image(const char *path, uint32_t *width,
                                  uint32_t *height);
extern uint32_t gapi_draw_image(uint32_t handle, double x, double y,
                                double width, double height);
extern uint32_t gapi_draw_image_part(uint32_t handle, uint32_t slice_x,
                                     uint32_t slice_y, uint32_t slice_width,
                                     uint32_t slice_height, double x, double y,
                                     double width, double height);
// text
extern uint32_t gapi_draw_text(double x, double y, double maxwidth,
                               double height, const char *txt, const char font,
                               uint8_t style, char align, uint8_t cr,
                               uint8_t cg, uint8_t cb, uint8_t ca);

// events
extern int32_t gapi_key_pressed(char t, char k);
