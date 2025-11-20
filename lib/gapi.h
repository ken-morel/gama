#ifndef GAMA_API_H
#define GAMA_API_H

extern int gapi_init();
extern void gapi_log(const char* message);
extern void gapi_yield();
extern void gapi_quit();

#endif // GAMA_API_H
