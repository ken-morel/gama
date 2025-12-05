#pragma once

#define gnothing NULL

#include "gapi.h"
#include <stdint.h>

typedef enum { GM_COLLIDER_CIRCLE, GM_COLLIDER_RECT } gmColliderType;
typedef struct {
  uint8_t is_active;
  uint8_t is_static;

  gmColliderType collider_type;
  gmPos position;
  gmPos velocity;
  gmPos acceleration;

  double width, height, radius;
  double mass;
  double restitution;
  double friction;
} gmBody;

gmBody gm_body_create(double mass, double x, double y, double w, double h,
                      gmColliderType c) {
  gmBody body = {
      .is_active = 1,
      .is_static = 0,
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

void gm_max_speed(gmBody *body, double max_speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed > max_speed) {
    double factor = max_speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}

void gm_min_speed(gmBody *body, double min_speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed < min_speed) {
    // Avoid division by zero if body is not moving.
    if (current_speed == 0)
      return;

    double factor = min_speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}
void gm_speed(gmBody *body, double speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed == speed)
    return;

  if (current_speed == 0) {
    // If speed is 0, we have no direction. Assume movement along X axis.
    body->velocity.x = speed;
    body->velocity.y = 0;
  } else {
    double factor = speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}
void gm_max_speed_anim(gmBody *body, double max_speed,
                       void animator(double *value, double target, double dt,
                                     double t),
                       double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed > max_speed) {
    double factor = max_speed / current_speed;
    double x_target = body->velocity.x * factor;
    double y_target = body->velocity.y * factor;
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}
void gm_min_speed_anim(gmBody *body, double min_speed,
                       void animator(double *value, double target, double dt,
                                     double t),
                       double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed < min_speed) {
    // Avoid division by zero if body is not moving.
    if (current_speed == 0)
      return;
    double factor = min_speed / current_speed;
    double x_target = body->velocity.x * factor;
    double y_target = body->velocity.y * factor;
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}

void gm_speed_anim(gmBody *body, double speed,
                   void animator(double *value, double target, double dt,
                                 double t),
                   double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed != speed) {
    double x_target, y_target;
    if (current_speed == 0) {
      // If speed is 0, we have no direction. Assume movement along X axis.
      x_target = speed;
      y_target = 0;
    } else {
      double factor = speed / current_speed;
      x_target = body->velocity.x * factor;
      y_target = body->velocity.y * factor;
    }
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}
gmBody gm_rectangle_body(double m, double x, double y, double w, double h) {
  return gm_body_create(m, x, y, w, h, GM_COLLIDER_RECT);
}
gmBody gm_circle_body(double m, double x, double y, double r) {
  return gm_body_create(m, x, y, r, r, GM_COLLIDER_CIRCLE);
}

int gm_body_contains(gmBody *body, double x, double y);
static inline int gm_hovered(gmBody *body) {
  return gm_body_contains(body, gm_mouse.position.x, gm_mouse.position.y);
}
static inline int gm_clicked(gmBody *body) {
  return gm_mouse.pressed && gm_hovered(body);
}

// mark hard limits, set both begin and end to 0 to ignore
static inline uint8_t gm_body_bound_clip(gmBody *body, double bx, double ex,
                                         double by, double ey) {

  int8_t exited = 0;
  if (bx != 0 || ex != 0) {
    if (body->position.x < bx) {
      body->position.x = bx;
      exited |= 0b1000;
    } else if (body->position.x > ex) {
      body->position.x = ex;
      exited |= 0b0100;
    }
  }
  if (by != 0 || ey != 0) {
    if (body->position.y < by) {
      body->position.y = by;
      exited |= 0b0010;
    } else if (body->position.y > ey) {
      body->position.y = ey;
      exited |= 0b0001;
    }
  }
  return exited;
}
// the body reenters on the other side if it's exits, set both begin and end to
// 0 to ignore
static inline int8_t gm_body_bound_reflect(gmBody *body, double bx, double ex,
                                           double by, double ey) {
  int8_t exited = 0;
  if (body->position.x < bx) {
    body->position.x = ex;
    exited |= 0b1000;
  } else if (body->position.x > ex) {
    body->position.x = bx;
    exited |= 0b0100;
  }
  if (body->position.y < by) {
    body->position.y = ey;
    exited |= 0b0010;
  } else if (body->position.y > ey) {
    body->position.y = by;
    exited |= 0b0001;
  }
  return exited;
}

// bounce the shape with the specified restitution if it exceeds
static inline int8_t gm_body_bound_bounce(gmBody *body, double bx, double ex,
                                          double by, double ey,
                                          double restitution) {
  int8_t exited = 0;
  if (body->position.x < bx && body->velocity.x < 0) {
    body->velocity.x = fabs(body->velocity.x) * restitution;
    exited |= 0b1000;
  } else if (body->position.x > ex && body->velocity.x > 0) {
    body->velocity.x = -fabs(body->velocity.x) * restitution;
    exited |= 0b0100;
  }
  if (body->position.y < by && body->velocity.y < 0) {
    body->velocity.y = fabs(body->velocity.y) * restitution;
    exited |= 0b0010;
  } else if (body->position.y > ey && body->velocity.y > 0) {
    body->velocity.y = -fabs(body->velocity.y) * restitution;
    exited |= 0b0001;
  }
  return exited;
}
