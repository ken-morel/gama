#pragma once

#include <stdlib.h>

typedef struct {
  double x, y, z;
} gama_position;

void gama_position_update(gama_position *p, gama_position *v, gama_position *a,
                          double dt) {
  double dt2 = dt * dt;
  double vx = 0, vy = 0, vz = 0;
  double ax = 0, ay = 0, az = 0;
  if (a != NULL) {
    ax = a->x;
    ay = a->y;
    az = a->z;
  }
  if (v != NULL) {
    vx = v->x;
    vy = v->y;
    vz = v->z;
  }
  p->x += vx + (ax * dt2);
  p->y += vy + (ay * dt2);
  p->z += vz + (az * dt2);

  if (v != NULL) {
    v->x += ax * dt;
    v->y += ay * dt;
    v->z += az * dt;
  }
}

void gama_position_set(gama_position *p, double x, double y) {
  p->x = x;
  p->y = y;
  p->z = 0;
}
void gama_position_set3(gama_position *p, double x, double y, double z) {
  p->x = x;
  p->y = y;
  p->z = z;
}
