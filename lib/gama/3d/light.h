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

const gm3Light gm3_default_light = {
    .position = {0, 1, 0},
    .direction = {0, -1, 1},
    .color = 0xCCCCCCFF,
    .intensity = 0.5,
    .ambient = 0.5,
};

int gm3_light_create(gm3Light *l) {
  if (!l)
    return -1;
  *l = gm3_default_light;
  memset(l, 0, sizeof(gm3Light));

  return 0;
}
