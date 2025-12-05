#pragma once

#include "body.h"
#include "body_list.h"
#include "position.h"

typedef struct {
  struct gm_system *sys;
  gmBody *bodies[2];
  double penetration;
  double since;
  gmPos normals;
} gmCollision;

typedef struct gm_system {
  int is_active;
  gmBodies bodies;

  gmCollision **collisions;

  gmPos velocity;
  gmPos acceleration;

  double damping;
} gmSystem;

gmSystem gm_system_create() {
  gmSystem sys = {.is_active = 1,
                  .bodies = NULL,
                  .velocity = {0, 0},
                  .acceleration = {0, 0},
                  .damping = 0,
                  .collisions = NULL};
  return sys;
}

static inline void gm_system_push(gmSystem *sys, gmBody *body) {
  sys->bodies = gm_bodies_push(sys->bodies, body);
}
static inline void gm_system_push2(gmSystem *sys, gmBody *a, gmBody *b) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
}

static inline void gm_system_push3(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
}
static inline void gm_system_push4(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c, gmBody *d) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
  gm_system_push(sys, d);
}
static inline void gm_system_push5(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c, gmBody *d, gmBody *e) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
  gm_system_push(sys, d);
  gm_system_push(sys, e);
}
static inline void gm_system_push_array(gmSystem *sys, size_t number,
                                        gmBody *bodies) {
  for (size_t i = 0; i < number; i++)
    gm_system_push(sys, &bodies[i]);
}
static inline void gm_system_pop(gmSystem *sys) {
  sys->bodies = gm_bodies_pop(sys->bodies);
}

static inline size_t gm_system_size(gmSystem *sys) {
  return gm_bodies_length(sys->bodies);
}
