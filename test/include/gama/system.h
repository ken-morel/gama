#pragma once

#include "body.h"
#include "body_list.h"
#include "position.h"

/**
 * @brief Structure representing a collision between two bodies.
 */
typedef struct {
  struct gm_system
      *sys;           /**< Pointer to the system where the collision occurred */
  gmBody *bodies[2];  /**< Array containing the two colliding bodies */
  double penetration; /**< Depth of penetration between the bodies */
  double since;       /**< Time since the collision began */
  gmPos normals;      /**< Normal vector of the collision */
} gmCollision;

/**
 * @brief Structure representing a physics system containing bodies and
 * collision information.
 */
typedef struct gm_system {
  int is_active;   /**< Whether the system is active */
  gmBodies bodies; /**< List of bodies in the system */

  gmCollision **collisions; /**< Array of collision information */

  gmPos velocity;     /**< Velocity applied to all bodies in the system */
  gmPos acceleration; /**< Acceleration applied to all bodies in the system */

  double damping; /**< Damping factor applied to all bodies in the system */
} gmSystem;

/**
 * @brief Creates a new physics system with default values.
 * @return A new gmSystem instance.
 */
gmSystem gm_system_create() {
  gmSystem sys = {.is_active = 1,
                  .bodies = NULL,
                  .velocity = {0, 0},
                  .acceleration = {0, 0},
                  .damping = 0,
                  .collisions = NULL};
  return sys;
}

/**
 * @brief Adds a body to the physics system.
 * @param sys Pointer to the system to add to.
 * @param body Pointer to the body to add.
 */
static inline void gm_system_push(gmSystem *sys, gmBody *body) {
  sys->bodies = gm_bodies_push(sys->bodies, body);
}

/**
 * @brief Adds two bodies to the physics system.
 * @param sys Pointer to the system to add to.
 * @param a Pointer to the first body to add.
 * @param b Pointer to the second body to add.
 */
static inline void gm_system_push2(gmSystem *sys, gmBody *a, gmBody *b) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
}

/**
 * @brief Adds three bodies to the physics system.
 * @param sys Pointer to the system to add to.
 * @param a Pointer to the first body to add.
 * @param b Pointer to the second body to add.
 * @param c Pointer to the third body to add.
 */
static inline void gm_system_push3(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
}

/**
 * @brief Adds four bodies to the physics system.
 * @param sys Pointer to the system to add to.
 * @param a Pointer to the first body to add.
 * @param b Pointer to the second body to add.
 * @param c Pointer to the third body to add.
 * @param d Pointer to the fourth body to add.
 */
static inline void gm_system_push4(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c, gmBody *d) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
  gm_system_push(sys, d);
}

/**
 * @brief Adds five bodies to the physics system.
 * @param sys Pointer to the system to add to.
 * @param a Pointer to the first body to add.
 * @param b Pointer to the second body to add.
 * @param c Pointer to the third body to add.
 * @param d Pointer to the fourth body to add.
 * @param e Pointer to the fifth body to add.
 */
static inline void gm_system_push5(gmSystem *sys, gmBody *a, gmBody *b,
                                   gmBody *c, gmBody *d, gmBody *e) {
  gm_system_push(sys, a);
  gm_system_push(sys, b);
  gm_system_push(sys, c);
  gm_system_push(sys, d);
  gm_system_push(sys, e);
}

/**
 * @brief Adds an array of bodies to the physics system.
 * @param sys Pointer to the system to add to.
 * @param number The number of bodies to add.
 * @param bodies Pointer to the array of bodies to add.
 */
static inline void gm_system_push_array(gmSystem *sys, size_t number,
                                        gmBody *bodies) {
  for (size_t i = 0; i < number; i++)
    gm_system_push(sys, &bodies[i]);
}

/**
 * @brief Removes the last body from the physics system.
 * @param sys Pointer to the system to remove from.
 */
static inline void gm_system_pop(gmSystem *sys) {
  sys->bodies = gm_bodies_pop(sys->bodies);
}

/**
 * @brief Gets the number of bodies in the system.
 * @param sys Pointer to the system to check.
 * @return The number of bodies in the system.
 */
static inline size_t gm_system_size(gmSystem *sys) {
  return gm_bodies_length(sys->bodies);
}

/**
 * @brief Destroy the system and free memory.
 * @param sys Pointer to the system to destroy.
 */
void gm_system_destroy(gmSystem *sys) {
  if (sys->collisions != NULL) {
    for (size_t i = 0; sys->collisions[i] != NULL; i++) {
      free(sys->collisions[i]);
    }
    free(sys->collisions);
  }
}
