#pragma once

#include "body.h"
#include "body_list.h"
#include "position.h"
typedef struct {
  int is_active;
  gmBodies bodies;


  gmPos velocity;
  gmPos acceleration;

  double damping;
} gmSystem;

gmSystem gm_system_create() {
  gmSystem sys = {.is_active = 1,
                  .bodies = NULL,
                  .velocity = {0, 0},
                  .acceleration = {0, 0},
                  .damping = 0
                };
  return sys;
}



static inline void gm_system_push(gmSystem *sys, gmBody *body) {
  sys->bodies = gm_bodies_push(sys->bodies, body);
}
static inline void gm_system_pop(gmSystem *sys) {
  sys->bodies = gm_bodies_pop(sys->bodies);
}

static inline size_t gm_system_size(gmSystem *sys) {
  return gm_bodies_length(sys->bodies);
}


