#pragma once

#include "_math.h"

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

/**
 * Calculate the area of a triangle formed by 3 2D points using the cross
 * product formula
 *
 * @param a First vertex of the triangle
 * @param b Second vertex of the triangle
 * @param c Third vertex of the triangle
 * @return The area of the triangle (always positive)
 */
double gm_triangle_area(gmPos a, gmPos b, gmPos c) {
  double ab_x = b.x - a.x;
  double ab_y = b.y - a.y;
  double ac_x = c.x - a.x;
  double ac_y = c.y - a.y;
  return 0.5 * fabs(ab_x * ac_y - ab_y * ac_x);
}
