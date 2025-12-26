#pragma once

#include "position.h"
#include <stdint.h>

struct _gmMouse {
  gmPos position;
  gmPos lastPosition;
  gmPos movement;

  uint8_t clicked;
  uint8_t down;
};

struct _gmMouse gm_mouse = {
    .position = {0, 0},
    .lastPosition = {0, 0},
    .movement = {0, 0},
    .clicked = 0,
    .down = 0,
};
