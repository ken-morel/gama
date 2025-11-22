#pragma once

#define GAMA_TEXT_MAX_SIZE 50

#include "position.h"

#define GAMA_BOLD 0b1
#define GAMA_MONO 0b10
#define GAMA_ITALIC 0b100

struct gama_text {
  char text[GAMA_TEXT_MAX_SIZE];
  char font[30];
  double size;
  double max_width;
  struct gama_position position;
  struct gama_position velocity;
  struct gama_position acceleration;
};



