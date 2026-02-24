#ifndef GM_PHYSICS_H_INCLUDED
#define GM_PHYSICS_H_INCLUDED
#include "_math.h"
#include "body.h"
#include "body_list.h"
#include "collision.h"
#include "position.h"
#include "system.h"
#include "t.h"

/**
 * @brief Resolves a collision between two bodies by applying appropriate forces
 * and corrections.
 * @param collision Pointer to the collision to resolve.
 */
void gm_collision_resolve(gmCollision *collision);

// ---------------------------------------------------------------------------
// ------------------------------ Core Physics Update ------------------------
// ---------------------------------------------------------------------------

/**
 * @brief Updates a single physics body's position and velocity based on applied
 * accelerations and damping.
 *
 * This function integrates the body's motion over a given time step. It
 * accounts for the body's own acceleration and any system-wide acceleration and
 * damping.
 *
 * @param sys Pointer to the physics system the body belongs to (can be NULL if
 *        no system-wide effects are desired).
 * @param body Pointer to the body to update.
 * @param dt The time step (delta time) for the update.
 */
void gm_system_update_body_dt(gmSystem *sys, gmBody *body, double dt) {
  if (body->is_static || body == NULL || (sys != NULL && !sys->is_active) ||
      !body->is_active) {
    return; // Don't update inactive or static bodies
  }
  // Step 1: Update velocity based on accelerations (both body and system)
  body->velocity.x += body->acceleration.x * dt;
  body->velocity.y += body->acceleration.y * dt;

  if (sys != NULL) {
    body->velocity.x += sys->acceleration.x * dt;
    body->velocity.y += sys->acceleration.y * dt;
  }

  // Step 2: Apply damping to velocity (after all force updates)
  if (sys != NULL) {
    double damp_factor = 1.0 / (1.0 + (sys->damping * dt));
    body->velocity.x *= damp_factor;
    body->velocity.y *= damp_factor;
  }

  // Step 3: Update position based on the new velocity
  body->position.x += body->velocity.x * dt;
  body->position.y += body->velocity.y * dt;

  if (sys != NULL) {
    body->position.x +=
        sys->velocity.x * dt; // Update position with system velocity
    body->position.y += sys->velocity.y * dt;
  }
}

/**
 * @brief Updates a single physics body's position and velocity using a
 * specified time step, without considering a global physics system.
 * @param body Pointer to the body to update.
 * @param dt The time step for the update.
 */
void gm_body_update_dt(gmBody *body, double dt) {
  return gm_system_update_body_dt(NULL, body, dt);
}

/**
 * @brief Updates a single physics body's position and velocity using the
 * engine's global delta time (`gm_dt()`), without considering a global physics
 * system.
 * @param body Pointer to the body to update.
 */
void gm_body_update(gmBody *body) {
  return gm_system_update_body_dt(NULL, body, gm_dt());
}

/**
 * @brief Checks if two given bodies are involved in the specified collision.
 *
 * This function determines if the provided collision object (`c`) involves
 * the two specified bodies (`a` and `b`), regardless of their order within the
 * collision object.
 *
 * @param c Pointer to the collision object to check.
 * @param a Pointer to the first body.
 * @param b Pointer to the second body.
 * @return 1 if the bodies are involved in the collision, 0 otherwise.
 */
static inline int gm_collision_bodies_are(gmCollision *c, gmBody *a,
                                          gmBody *b) {
  return c->bodies[0] == a && c->bodies[1] == b ||
         c->bodies[0] == b && c->bodies[1] == a;
}

/**
 * @brief Updates a physics system over a given total time step,
 * performing sub-steps for stable collision detection and resolution.
 *
 * This is the main update function for a physics system. It integrates
 * the motion of all bodies, detects new collisions, resolves them, and
 * manages the lifecycle of collision objects.
 *
 * @param sys Pointer to the system to update.
 * @param unit The duration of each sub-step for physics integration.
 * @param dt The total time duration to simulate in this update.
 */
void gm_system_update_dt(gmSystem *sys, double unit, double dt) {
  if (sys == NULL || !sys->is_active)
    return;

  gmCollision **newCollisions = NULL;
  gmCollision **prevCollisions = sys->collisions;

  const size_t subSteps = fmax(dt / unit, 1);
  const size_t count = gm_system_size(sys);
  const double subdt = dt / (double)subSteps;

  for (int i = 0; i < subSteps; i++) {
    for (int j = 0; j < count; j++)
      gm_system_update_body_dt(sys, sys->bodies[j], subdt);

    for (int j = 0; j < count; j++) {
      for (int k = j + 1; k < count; k++) {
        if (!sys->bodies[j]->is_active || !sys->bodies[k]->is_active) {
          continue;
        }

        gmCollision *collision = malloc(sizeof(gmCollision));
        if (gm_collision_detect(collision, sys->bodies[j], sys->bodies[k])) {
          collision->sys = sys;
          gm_collision_resolve(collision);
          newCollisions = (gmCollision **)gm_ptr_list_push(
              (gmPtrList)newCollisions, collision);
        }
      }
    }
  }

  gmCollision *prevC, *newC;
  gm_ptr_list_for_each(prevC, prevCollisions) {
    int found = 0;
    gm_ptr_list_for_each(newC, newCollisions) {
      if (gm_collision_bodies_are(prevC, newC->bodies[0], newC->bodies[1])) {
        newC->since = prevC->since + dt;
        found = 1;
        break;
      }
    }
    if (!found)
      free(prevC);
  }

  if (prevCollisions)
    free(prevCollisions);

  sys->collisions = newCollisions;
}

/**
 * @brief Gets the collision information for two specific bodies in a system.
 *
 * This function searches the system's active collisions to find one involving
 * the two specified bodies.
 *
 * @param collision Pointer to a `gmCollision` struct where the found collision
 *        data will be copied. Can be NULL if only checking for existence.
 * @param sys Pointer to the system to search in.
 * @param a Pointer to the first body.
 * @param b Pointer to the second body.
 * @return 1 if a collision involving `a` and `b` is found, 0 otherwise.
 */
int gm_system_get_collision(gmCollision *collision, gmSystem *sys, gmBody *a,
                            gmBody *b) {
  gmCollision *coll;
  gm_ptr_list_for_each(coll, sys->collisions) {
    if (gm_collision_bodies_are(coll, a, b)) {
      if (collision != NULL)
        *collision = *coll;
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Updates the physics system using a specified time unit and the
 * engine's global delta time (`gm_dt()`).
 * @param sys Pointer to the system to update.
 * @param unit The time unit for sub-step calculations.
 */
static inline void gm_system_update_unit(gmSystem *sys, double unit) {
  return gm_system_update_dt(sys, unit, gm_dt());
}

/**
 * @brief Default time step for physics system frame updates.
 *
 * This value determines the granularity of physics calculations per frame.
 * A smaller value leads to more accurate (and potentially slower) simulations.
 */
double gm_system_frame_time = 0.001; // ~15 updates per frame.

/**
 * @brief Updates the physics system using the default `gm_system_frame_time`.
 * @param sys Pointer to the system to update.
 */
static inline void gm_system_update(gmSystem *sys) {
  return gm_system_update_unit(sys, gm_system_frame_time);
}

// ---------------------------------------------------------------------------
// ------------------------------ Collision Response -------------------------
// ---------------------------------------------------------------------------

/**
 * @brief Calculates the penetration depth and optionally the normal vector
 *         for a collision between two bodies.
 *
 * This function handles collision between various collider types
 * (Circle-Circle, Rect-Rect, Circle-Rect). It determines how much the bodies
 * overlap and the direction of the separation.
 *
 * @param a Pointer to the first body.
 * @param b Pointer to the second body.
 * @param normal_x Pointer to store the x component of the collision normal (can
 *        be NULL if not needed). The normal points from body `a` to body `b`.
 * @param normal_y Pointer to store the y component of the collision normal (can
 *        be NULL if not needed). The normal points from body `a` to body `b`.
 * @return The penetration depth between the bodies. A positive value indicates
 *         overlap.
 */
double gm_collision_penetration_normals(gmBody *a, gmBody *b, double *normal_x,
                                        double *normal_y) {
  double penetration_depth = 0; // Initialize to 0

  // CASE: Circle vs Circle
  if (a->collider_type == GM_COLLIDER_CIRCLE &&
      b->collider_type == GM_COLLIDER_CIRCLE) {
    double dx = b->position.x - a->position.x;
    double dy = b->position.y - a->position.y;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance == 0) {
      distance = 0.001;
      dx = 0.001; // Avoid division by zero
    }
    penetration_depth = a->radius + b->radius - distance;
    if (penetration_depth > 0 && normal_x != NULL && normal_y != NULL) {
      *normal_x = dx / distance;
      *normal_y = dy / distance;
    }
  }
  // CASE: Rect vs Rect
  else if (a->collider_type == GM_COLLIDER_RECT &&
           b->collider_type == GM_COLLIDER_RECT) {
    double dx = b->position.x - a->position.x;
    double dy = b->position.y - a->position.y;

    double overlap_x = (a->width / 2 + b->width / 2) - fabs(dx);
    if (overlap_x <= 0)
      return 0; // No overlap in X

    double overlap_y = (a->height / 2 + b->height / 2) - fabs(dy);
    if (overlap_y <= 0)
      return 0; // No overlap in Y

    if (normal_x != NULL && normal_y != NULL) {
      if (overlap_x < overlap_y) {
        penetration_depth = overlap_x;
        *normal_x = (dx < 0) ? -1 : 1;
        *normal_y = 0;
      } else {
        penetration_depth = overlap_y;
        *normal_x = 0;
        *normal_y = (dy < 0) ? -1 : 1;
      }
    }
  }
  // CASE: Circle vs Rectangle
  else {
    // Determine which is circle and which is rect
    gmBody *circle = (a->collider_type == GM_COLLIDER_CIRCLE) ? a : b;
    gmBody *rect = (a->collider_type == GM_COLLIDER_RECT) ? a : b;

    double half_w = rect->width * 0.5;
    double half_h = rect->height * 0.5;

    // Find the point on the rectangle edge closest to the circle center
    double closest_x =
        fmax(rect->position.x - half_w,
             fmin(circle->position.x, rect->position.x + half_w));

    double closest_y =
        fmax(rect->position.y - half_h,
             fmin(circle->position.y, rect->position.y + half_h));

    double dx = circle->position.x - closest_x;
    double dy = circle->position.y - closest_y;
    double distance_sq = dx * dx + dy * dy;

    // Check if collision occurred (distance < radius)
    if (distance_sq < circle->radius * circle->radius) {
      double distance = sqrt(distance_sq);

      // Sub-case A: Center is OUTSIDE the rectangle
      if (distance > 0.0001 && normal_x != NULL && normal_y != NULL) {
        // Normal points from Rect Surface -> Circle Center
        *normal_x = dx / distance;
        *normal_y = dy / distance;
        penetration_depth = circle->radius - distance;
      }
      // Sub-case B: Center is INSIDE the rectangle (special handling)
      else {
        // Calculate distance to all 4 edges to find the shortest path out
        double left_pen = circle->position.x - (rect->position.x - half_w);
        double right_pen = (rect->position.x + half_w) - circle->position.x;
        double bottom_pen = circle->position.y - (rect->position.y - half_h);
        double top_pen = (rect->position.y + half_h) - circle->position.y;

        double min_x = fmin(left_pen, right_pen);
        double min_y = fmin(bottom_pen, top_pen);

        // We add radius because we need to push the circle entirely out
        // so its edge touches the rect edge, not just its center.
        if (normal_x != NULL && normal_y != NULL) {
          if (min_x < min_y) {
            penetration_depth = min_x + circle->radius;
            *normal_x = (left_pen < right_pen) ? -1 : 1;
            *normal_y = 0;
          } else {
            penetration_depth = min_y + circle->radius;
            *normal_x = 0;
            *normal_y = (bottom_pen < top_pen) ? -1 : 1;
          }
        }
      }

      // --- CRITICAL FIX ---
      // The logic above calculates a normal pointing from the rectangle surface
      // towards the circle center. We must ensure the final 'normal' used for
      // resolution points from body A to body B.
      if (a == circle) {
        // Current normal is B(Rect) -> A(Circle). We want A -> B. Invert.
        *normal_x = -*normal_x;
        *normal_y = -*normal_y;
      }
      // If a is Rect, current normal is A(Rect) -> B(Circle). Keep as is.
    }
  }
  return penetration_depth;
}

/**
 * @brief Calculates the penetration depth for a collision between two bodies.
 *
 * This function is a simplified version of `gm_collision_penetration_normals`
 * that only returns the depth and does not calculate the normal vector.
 *
 * @param a Pointer to the first body.
 * @param b Pointer to the second body.
 * @return The penetration depth between the bodies. A positive value indicates
 *         overlap.
 */
double gm_collision_penetration(gmBody *a, gmBody *b) {
  return gm_collision_penetration_normals(a, b, NULL, NULL);
}

/**
 * @brief Resolves a collision by adjusting positions and velocities of
 * colliding bodies.
 * @param coll Pointer to the collision to resolve.
 */
void gm_collision_resolve(gmCollision *coll) {
  if (coll == NULL)
    return;
  gmBody *a = coll->bodies[0];
  gmBody *b = coll->bodies[1];
  coll->penetration = gm_collision_penetration_normals(a, b, &coll->normals.x,
                                                       &coll->normals.y);

  if (coll->penetration <= 0) {
    return; // No collision to resolve
  }

  // --- 2. Resolve Velocity ---
  double rel_vx = b->velocity.x - a->velocity.x;
  double rel_vy = b->velocity.y - a->velocity.y;
  double vel_along_normal = rel_vx * coll->normals.x + rel_vy * coll->normals.y;

  // Do not resolve if velocities are separating
  if (vel_along_normal > 0) {
    return;
  }

  double e = fmin(a->restitution, b->restitution);
  double j = -(1 + e) * vel_along_normal;

  double inv_mass_a = (a->mass > 0) ? 1.0 / a->mass : 0;
  double inv_mass_b = (b->mass > 0) ? 1.0 / b->mass : 0;

  if (inv_mass_a + inv_mass_b == 0)
    return;

  j /= (inv_mass_a + inv_mass_b);

  double impulse_x = j * coll->normals.x;
  double impulse_y = j * coll->normals.y;

  if (a->mass > 0) {
    a->velocity.x -= inv_mass_a * impulse_x;
    a->velocity.y -= inv_mass_a * impulse_y;
  }
  if (b->mass > 0) {
    b->velocity.x += inv_mass_b * impulse_x;
    b->velocity.y += inv_mass_b * impulse_y;
  }

  // --- 3. Positional Correction ---
  const double percent = 0.2; // Percentage of penetration to correct
  const double slop = 0.01;   // Penetration allowance to prevent jitter
  double correction_amount =
      fmax(coll->penetration - slop, 0.0) / (inv_mass_a + inv_mass_b) * percent;

  double correction_x = correction_amount * coll->normals.x;
  double correction_y = correction_amount * coll->normals.y;

  if (a->mass > 0) {
    a->position.x -= inv_mass_a * correction_x;
    a->position.y -= inv_mass_a * correction_y;
  }
  if (b->mass > 0) {
    b->position.x += inv_mass_b * correction_x;
    b->position.y += inv_mass_b * correction_y;
  }
}

#endif // GM_PHYSICS_H_INCLUDED
