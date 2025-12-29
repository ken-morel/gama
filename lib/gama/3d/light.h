#pragma once

#include "../color.h"
#include "position.h"

typedef struct {
  gm3Pos position;
  gm3Pos direction;
  gmColor color;
  double intensity;
  double ambient;
} gm3Light;

int gm3_light_create(gm3Light *l) {
  if (!l)
    return -1;
  memset(l, 0, sizeof(gm3Light));

  l->position.y = 1;

  l->direction.z = 1;
  l->direction.y = -1;

  l->color = 0xCCCCCCFF;

  l->intensity = 1;

  l->ambient = 1;
  return 0;
}
