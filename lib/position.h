#pragma once

#include "math.h"
typedef struct {
  double x, y;
} gmPos;

static inline gmPos gmpos(double x, double y) {
  gmPos p = {x, y};
  return p;
}

static inline void gm_pos_set(gmPos *p, double x, double y) {
  p->x = x;
  p->y = y;
}

static inline void gm_pos_reset(gmPos *p) {
  p->x = 0;
  p->y = 0;
}

static inline double gm_pos_magniture(gmPos p) {
  return sqrt(p.x * p.x + p.y * p.y);
}

static inline double gm_pos_distance(gmPos a, gmPos b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
