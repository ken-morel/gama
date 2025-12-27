#pragma once

#include "position.h"

typedef struct {
  gm3Pos position;
  gm3Pos rotation;
  gm3Pos scale;
} gm3Transform;

gm3Pos gm3_pos_rotate(gm3Pos p, gm3Pos rot) {
  gm3Pos res = p;
  double temp;

  // 1. Rotate around X-axis
  temp = res.y * cos(rot.x) - res.z * sin(rot.x);
  res.z = res.y * sin(rot.x) + res.z * cos(rot.x);
  res.y = temp;

  // 2. Rotate around Y-axis
  temp = res.x * cos(rot.y) + res.z * sin(rot.y);
  res.z = -res.x * sin(rot.y) + res.z * cos(rot.y);
  res.x = temp;

  // 3. Rotate around Z-axis
  temp = res.x * cos(rot.z) - res.y * sin(rot.z);
  res.y = res.x * sin(rot.z) + res.y * cos(rot.z);
  res.x = temp;

  return res;
}

gm3Pos gm3_pos_translate(gm3Pos p, gm3Pos trans) {
  gm3Pos res = p;
  res.x += trans.x;
  res.y += trans.y;
  res.z += trans.z;
  return res;
}

gm3Pos gm3_pos_scale(gm3Pos p, gm3Pos trans) {
  gm3Pos res = p;
  res.x *= trans.x;
  res.y *= trans.y;
  res.z *= trans.z;
  return res;
}
