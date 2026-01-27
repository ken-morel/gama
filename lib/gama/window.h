#ifndef GAMA_WINDOW_H_INCLUDED
#define GAMA_WINDOW_H_INCLUDED

#include <stdint.h>
struct {
  uint32_t width, height;
  uint32_t prevWidth, prevHeight;

  int resized;
} gmWindow;

#endif // GAMA_WINDOW_H_INCLUDED
