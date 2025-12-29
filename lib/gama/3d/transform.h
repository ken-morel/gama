#pragma once

#include "position.h"

typedef struct {
  gm3Pos position;
  gm3Pos rotation;
  gm3Pos scale;
} gm3Transform;

void gm3_pos_rotate(gm3Pos *res, const gm3Pos *rot) {
  if (!res || !rot)
    return;
  double temp;

  // 1. Rotate around X-axis
  temp = res->y * cos(rot->x) - res->z * sin(rot->x);
  res->z = res->y * sin(rot->x) + res->z * cos(rot->x);
  res->y = temp;

  // 2. Rotate around Y-axis
  temp = res->x * cos(rot->y) + res->z * sin(rot->y);
  res->z = -res->x * sin(rot->y) + res->z * cos(rot->y);
  res->x = temp;

  // 3. Rotate around Z-axis
  temp = res->x * cos(rot->z) - res->y * sin(rot->z);
  res->y = res->x * sin(rot->z) + res->y * cos(rot->z);
  res->x = temp;
}
void gm3_transform_pos(gm3Pos *p, gm3Transform *t) {
  gm3_pos_mul(p, &t->scale);
  gm3_pos_rotate(p, &t->rotation);
  gm3_pos_add(p, &t->position);
}
gm3Transform gm3_transform() {
  gm3Transform t;
  gm3_pos_reset(&t.position);
  gm3_pos_reset(&t.rotation);
  t.scale.x = 1;
  t.scale.y = 1;
  t.scale.z = 1;
  return t;
}
