#pragma once
#include <stdint.h>

extern int32_t gapi_init(int32_t width, int32_t height, const char *title);
extern void gapi_log(const char *message);
extern int gapi_yield(double *theta);
extern void gapi_quit();
extern int32_t gapi_runs();

 extern int32_t gapi_draw_rect(double x, double y, double w, double h,
                               uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca);

