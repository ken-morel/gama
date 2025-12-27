#pragma once

#include "../color.h"
#include "position.h"
typedef struct {
  gm3Pos position;
  gm3Pos direction;
  gmColor color;
  double intensity;
} gm3Light;
