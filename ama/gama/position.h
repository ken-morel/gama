#pragma once

#include <stdlib.h>

typedef struct {
  double x, y, z;
} gama_position;

void gama_position_set(gama_position *p, double x, double y) {
  p->x = x;
  p->y = y;
  p->z = 0;
}
void gama_position_set3(gama_position *p, double x, double y, double z) {
  p->x = x;
  p->y = y;
  p->z = z;
}
void gama_position_reset(gama_position *p) {
  p->x = 0;
  p->y = 0;
  p->z = 0;
}
