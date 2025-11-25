#pragma once

#include <stddef.h>
#include <stdlib.h>
#define gnothing NULL

#include "position.h"
#include <stdint.h>
#include <stdlib.h>
#include "gapi.h"


typedef enum { GM_COLLIDER_CIRCLE, GM_COLLIDER_RECT } gmColliderType;
typedef struct {
  int is_active;

  gmColliderType collider_type;
  gmPos position;
  gmPos velocity;
  gmPos acceleration;

  double width, height, radius;
  double mass; // ==0 -> static, >0 -> dynamic
  double restitution;
  double friction;
} gmBody;

gmBody gm_body_create(double mass,double x, double y, double w, double h,
                      gmColliderType c) {
  gmBody body = {
      .is_active = 1,
      .collider_type = c,
      .position = {.x = x, .y = y},
      .velocity = {.x = 0, .y = 0},
      .acceleration = {.x = 0, .y = 0},
      .width = w,
      .height = h,
      .radius = w < h ? w : h,
      .mass = mass,
      .restitution = 1,
  };
  return body;
}

gmBody gm_rectangle_body(double m,double x, double y, double w, double h) {
  return gm_body_create(m, x, y, w, h, GM_COLLIDER_RECT);
}
gmBody gm_circle_body(double m, double x, double y, double r) {
  return gm_body_create(m, x, y, r, r, GM_COLLIDER_CIRCLE);
}

#include "collision.h"

static inline int gm_hovered(gmBody *body) {
  return gm_body_contains(body, gm_mouse.position.x, gm_mouse.position.y);
}
static inline int gm_clicked(gmBody *body) {
  return gm_mouse.pressed && gm_hovered(body);
}
