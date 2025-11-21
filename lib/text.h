#pragma once

#include "position.h"

#define GAMA_BOLD 0b1
#define GAMA_MONO 0b10
#define GAMA_ITALIC 0b100

struct gama_text {
  char* text;
  struct gama_position position;
  struct gama_position velocity;
  struct gama_position acceleration;
};

