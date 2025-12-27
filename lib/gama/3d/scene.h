#pragma once

#include "../position.h"
#include "light.h"

typedef struct {
  gmPos viewport;
  double near;
  double far;
  gm3Light light;

} gm3Scene;
