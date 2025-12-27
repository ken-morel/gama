#pragma once

#include "../_math.h"
#include "../position.h"
#include <stddef.h>

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

static inline gm3Pos gm3_pos_center(gm3Pos va, gm3Pos vb) {
  gm3Pos center = {(va.x + vb.x) / 2.0, (va.y + vb.y) / 2.0,
                   (va.z + vb.z) / 2.0};
  return center;
}
static inline gm3Pos gm3_pos_center3(gm3Pos va, gm3Pos vb, gm3Pos vc) {
  gm3Pos center = {(va.x + vb.x + vc.x) / 3.0, (va.y + vb.y + vc.y) / 3.0,
                   (va.z + vb.z + vc.z) / 3.0};
  return center;
}

static inline gm3Pos gm3_pos_minus(gm3Pos va, gm3Pos vb) {
  gm3Pos diff = {va.x - vb.x, va.y - vb.y, va.z - vb.z};
  return diff;
}
static inline gm3Pos gm3_pos_plus(gm3Pos va, gm3Pos vb) {
  gm3Pos diff = {va.x + vb.x, va.y + vb.y, va.z + vb.z};
  return diff;
}

static inline gm3Pos gm3_pos_normalize(gm3Pos v) {
  double m = gm3_pos_magnitude(v);
  gm3Pos norm = {v.x / m, v.y / m, v.z / m};
  return norm;
}

static inline double gm3_pos_dot(gm3Pos a, gm3Pos b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline gm3Pos gm3_pos_cross(gm3Pos a, gm3Pos b) {
  return (gm3Pos){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x};
}

static inline void gm3_pos_reset(gm3Pos *p) {
  p->x = 0;
  p->y = 0;
  p->z = 0;
}

static inline gm3Pos gm3_pos_centerN(gm3Pos *arr, size_t n) {
  double dn = (double)n;
  gm3Pos c = {0, 0, 0};
  for (size_t i = 0; i < n; i++) {
    c.x += arr[i].x / dn;
    c.y += arr[i].y / dn;
    c.z += arr[i].z / dn;
  }
  return c;
}
