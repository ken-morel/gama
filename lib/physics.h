#include "position.h"

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

