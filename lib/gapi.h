#ifndef GAMA_API_H
#define GAMA_API_H
#include <stdint.h>

extern int32_t gapi_init(int32_t width, int32_t height, const char *title);
extern void gapi_log(const char *message);
extern int gapi_yield(double *theta);
extern void gapi_quit();
extern int32_t gapi_runs();

#endif // GAMA_API_H
