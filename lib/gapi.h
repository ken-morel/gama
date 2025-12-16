#pragma once
#include <stdint.h>

#include "position.h"

double _gm_dt = 0;
double _gm_t = 0;

static inline double gm_dt() { return _gm_dt; }
static inline double gm_t() { return _gm_t; }

struct {
  gmPos position;
  gmPos movement;

  int32_t pressed;
  int32_t down;
} gm_mouse = {
    .position =
        {
            .x = 0,
            .y = 0,
        },
    .movement =
        {
            .x = 0,
            .y = 0,
        },
    .pressed = 0,
    .down = 0,
};

// --- Windowing ---
extern void gapi_set_title(const char *title);
extern void gapi_resize(int32_t width, int32_t height);
extern void gapi_set_bg_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
extern void gapi_fullscreen(int32_t fullscreen);

// --- Utils ---
extern void gapi_log(const char *message);

// --- Game Loop ---
extern int32_t gapi_init(int32_t width, int32_t height, const char *title);
extern int32_t gapi_yield(double *dt); // Changed to int32_t return type
extern void gapi_quit();
extern int32_t gapi_runs();

// --- Drawing Primitives ---
extern int32_t gapi_draw_line(double x1, double y1, double x2, double y2,
                              double thickness, uint8_t r, uint8_t g, uint8_t b,
                              uint8_t a);
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

// --- Image Functions ---
extern uint32_t gapi_create_image(const char *path, uint32_t *width,
                                  uint32_t *height);
extern int32_t gapi_draw_image(uint32_t handle, double x, double y,
                               double width, double height);
extern int32_t gapi_draw_image_part(uint32_t handle, uint32_t slice_x,
                                    uint32_t slice_y, uint32_t slice_width,
                                    uint32_t slice_height, double x, double y,
                                    double width, double height);

// --- Text Functions ---
extern int32_t gapi_draw_text(double x, double y, double height,
                              const char *txt, const char *font, uint8_t style,
                              uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca);

// --- Event Functions ---
extern int32_t gapi_key_pressed(char t, char k);

extern void gapi_wait_queue();

extern void gapi_get_mouse_move(double *x, double *y);
extern int32_t gapi_mouse_down();
extern int32_t gapi_mouse_pressed();
extern int32_t gapi_mouse_get(double *x, double *y);
