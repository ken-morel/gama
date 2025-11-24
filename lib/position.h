#pragma once

typedef struct {
  double x, y;
} gmPos;

static inline gmPos gmp(double x, double y) {
  gmPos p = {x, y};
  return p;
}

void gm_pos_set(gmPos *p, double x, double y) {
  p->x = x;
  p->y = y;
}

void gm_pos_reset(gmPos *p) {
  p->x = 0;
  p->y = 0;
}
