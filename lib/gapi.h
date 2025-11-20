#ifndef GAMA_API_H
#define GAMA_API_H
#include <stdint.h>

extern int gapi_init();
extern void gapi_log(const char* message);
extern double gapi_yield();
extern void gapi_quit();
extern int gapi_runs();

#endif // GAMA_API_H
