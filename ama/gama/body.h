
#pragma once

#include <stddef.h>
#include <stdlib.h>
#define gnothing NULL

#include "position.h"
#include <stdint.h>
#include <stdlib.h>

typedef enum { GAMA_COLLIDER_CIRCLE, GAMA_COLLIDER_RECT } gama_collider_type;
typedef struct {
  int is_active;

  gama_collider_type collider_type;

  gama_position position;
  gama_position velocity;
  gama_position acceleration;

  double width, height, radius;
  double mass; // ==0 -> static, >0 -> dynamic
  double restitution;
  double friction;
} gama_body;

void gama_body_reset(gama_body *b) {
  b->is_active = 1;
  b->mass = 0;
  b->restitution = 1;
  b->friction = 1;
  b->width = 1;
  b->height = 1;
  b->radius = 1;
  b->collider_type = GAMA_COLLIDER_RECT;
  gama_position_reset(&b->position);
  gama_position_reset(&b->acceleration);
  gama_position_reset(&b->velocity);
}

gama_body gama_body_create(double x, double y, double w, double h,
                           gama_collider_type c) {
  gama_body body = {
      .is_active = 1,
      .collider_type = GAMA_COLLIDER_RECT,
      .position = {.x = x, .y = y, .z = 0},
      .velocity = {.x = 0, .y = 0, .z = 0},
      .acceleration = {.x = 0, .y = 0, .z = 0},
      .width = w,
      .height = h,
      .radius = w < h ? w : h,
      .mass = 1,
      .restitution = 0,
  };
  return body;
}

gama_body gama_rectangle_create(double x, double y, double w, double h) {
  return gama_body_create(x, y, w, h, GAMA_COLLIDER_RECT);
}
gama_body gama_circle_create(double x, double y, double r) {
  return gama_body_create(x, y, r, r, GAMA_COLLIDER_CIRCLE);
}
