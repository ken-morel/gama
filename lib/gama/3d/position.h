#pragma once

#include "../_math.h"
#include "../position.h"

typedef struct {
  double x, y, z;
} gm3Pos;

static inline gm3Pos gm3pos(double x, double y, double z) {
  gm3Pos p = {x, y, z};
  return p;
}

static inline void gm3_pos_set(gm3Pos *p, double x, double y, double z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

static inline double gm3_pos_magnitude(gm3Pos p) {
  return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}
static inline double gm3_pos_distance(gm3Pos a, gm3Pos b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

static inline gm3Pos gm3_pos_from2(gmPos p, double z) {
  return gm3pos(p.x, p.y, z);
}

static inline gmPos gm3_pos_project_simple(gm3Pos p) {
  return gmpos(p.x / p.z, p.y / p.z);
}
