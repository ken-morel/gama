/**
 * @file system.h
 * @brief Manages physics bodies, their interactions, and collision detection within a simulation.
 *
 * This file defines the `gmSystem` structure and functions for creating,
 * populating, and destroying a physics simulation environment.
 */
#ifndef GM_SYSTEM_H_INCLUDED
#define GM_SYSTEM_H_INCLUDED
#include "body.h"
#include "body_list.h"
#include "position.h"

struct gm_collision ;

/**
 * @brief Structure representing a physics system containing bodies and
 * collision information.
 */
typedef struct gm_system {
  int is_active;   /**< Whether the system is active and should be updated. */
  gmBodies bodies; /**< List of pointers to bodies currently managed by this system. */

  struct gm_collision **collisions; /**< Array of active collision information objects. */

  gmPos velocity;     /**< Global velocity applied to all bodies in the system (e.g., wind). */
  gmPos acceleration; /**< Global acceleration applied to all bodies (e.g., gravity). */

  double damping; /**< Global damping factor applied to all bodies (reduces velocity over time). */
} gmSystem;

#include "collision.h" // Include collision.h for gmCollision definition
/**
 * @brief Creates a new physics system with default values.
 *
 * Initializes a `gmSystem` with no bodies, zero global velocity/acceleration,
 * and a damping factor of 0.
 *
 * @return A new `gmSystem` instance.
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
 *
 * This function adds a pointer to a `gmBody` to the system's internal list.
 * The caller remains responsible for allocating and freeing the `gmBody` itself.
 *
 * @param sys Pointer to the system to add the body to.
 * @param body Pointer to the `gmBody` to add.
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
 * @param number The number of bodies in the array.
 * @param bodies Pointer to the array of `gmBody` instances to add.
 */
static inline void gm_system_push_array(gmSystem *sys, size_t number,
                                        gmBody *bodies) {
  for (size_t i = 0; i < number; i++)
    gm_system_push(sys, &bodies[i]);
}

/**
 * @brief Removes the last body pointer from the physics system's internal list.
 *
 * This function only removes the pointer; it does NOT free the `gmBody` itself.
 *
 * @param sys Pointer to the system to remove from.
 */
static inline void gm_system_pop(gmSystem *sys) {
  sys->bodies = gm_bodies_pop(sys->bodies);
}

/**
 * @brief Gets the number of bodies currently managed by the physics system.
 * @param sys Pointer to the system to check.
 * @return The number of bodies in the system.
 */
static inline size_t gm_system_size(gmSystem *sys) {
  return gm_bodies_length(sys->bodies);
}

/**
 * @brief Destroys the physics system and frees its internal memory.
 *
 * This function frees the memory allocated for the system's internal collision
 * list and the list of body pointers. It does NOT free the `gmBody` instances
 * themselves, which must be managed by the caller.
 *
 * @param sys Pointer to the system to destroy.
 */
void gm_system_destroy(gmSystem *sys) {
  if (sys->collisions != NULL) {
    for (size_t i = 0; sys->collisions[i] != NULL; i++) {
      free(sys->collisions[i]);
    }
    free(sys->collisions);
  }
  // Also clear the bodies list itself
  gm_bodies_clear(sys->bodies);
  // Do not free individual gmBody pointers, as they are owned by the caller.
}

#endif // GM_SYSTEM_H_INCLUDED
