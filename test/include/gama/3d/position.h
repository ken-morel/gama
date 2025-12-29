#pragma once

#include "../_math.h"
#include "../position.h"
#include <stddef.h>
#include <string.h>

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

static inline double gm3_pos_magnitude(const gm3Pos *p) {
  return sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
}
static inline double gm3_pos_distance(const gm3Pos *a, const gm3Pos *b) {
  return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}

static inline gm3Pos gm3_pos_from2(const gmPos *p, double z) {
  return gm3pos(p->x, p->y, z);
}

static inline gmPos gm3_pos_project_simple(const gm3Pos *p) {
  return gmpos(p->x / p->z, p->y / p->z);
}

static inline void gm3_pos_center(gm3Pos *p, const gm3Pos *va,
                                  const gm3Pos *vb) {
  p->x = (va->x + vb->x) / 2.0;
  p->y = (va->y + vb->y) / 2.0;
  p->z = (va->z + vb->z) / 2.0;
}
static inline void gm3_pos_center3(gm3Pos *p, const gm3Pos *va,
                                   const gm3Pos *vb, const gm3Pos *vc) {
  p->x = (va->x + vb->x + vc->x) / 3.0;
  p->y = (va->y + vb->y + vc->y) / 3.0;
  p->z = (va->z + vb->z + vc->z) / 3.0;
}

static inline void gm3_pos_substract(gm3Pos *va, const gm3Pos *vb) {
  va->x -= vb->x;
  va->y -= vb->y;
  va->z -= vb->z;
}

static inline void gm3_pos_add(gm3Pos *va, const gm3Pos *vb) {
  va->x += vb->x;
  va->y += vb->y;
  va->z += vb->z;
}

static inline void gm3_pos_normalize(gm3Pos *v) {
  double m = gm3_pos_magnitude(v);
  v->x /= m;
  v->y /= m;
  v->z /= m;
}

static inline double gm3_pos_dot(const gm3Pos *a, const gm3Pos *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline void gm3_pos_cross(gm3Pos *res, const gm3Pos *a,
                                 const gm3Pos *b) {
  res->x = a->y * b->z - a->z * b->y;
  res->y = a->z * b->x - a->x * b->z;
  res->z = a->x * b->y - a->y * b->x;
}

static inline void gm3_pos_reset(gm3Pos *p) { memset(p, 0, sizeof(*p)); }

static inline void gm3_pos_centerN(gm3Pos *res, const gm3Pos *arr, size_t n) {
  double dn = (double)n;
  for (size_t i = 0; i < n; i++) {
    res->x += arr[i].x / dn;
    res->y += arr[i].y / dn;
    res->z += arr[i].z / dn;
  }
}

void gm3_pos_mul(gm3Pos *res, gm3Pos *trans) {
  res->x *= trans->x;
  res->y *= trans->y;
  res->z *= trans->z;
}
void gm3_pos_mul_scalar(gm3Pos *res, double s) {
  res->x *= s;
  res->y *= s;
  res->z *= s;
}
