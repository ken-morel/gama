#ifndef GAMA_RECT_H_INCLUDED
#define GAMA_RECT_H_INCLUDED

#include "position.h"

typedef struct {
  gmPos pos;
  gmPos size;
} gmRect;

static inline gmRect gmrect(double x, double y, double w, double h) {
  return (gmRect){{x, y}, {w, h}};
}
static inline gmRect gmrectp(gmPos pos, gmPos size) {
  return (gmRect){pos, size};
}

typedef struct {
  gmPos pos;
  double r;
} gmCirc;

static inline gmCirc gmcirc(double x, double y, double r) {
  return (gmCirc){{x, y}, r};
}
static inline gmCirc gmcircp(gmPos p, double r) { return (gmCirc){p, r}; }

#endif
