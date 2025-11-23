#pragma once

#include "body.h"
#include "gapi.h"
#include "position.h"
#include <math.h>
#include <stdio.h>

// --- Forward declarations for collision functions ---
int gama_collision_detect(gama_body *a, gama_body *b);
void gama_collision_resolve(gama_body *a, gama_body *b);

// ---------------------------------------------------------------------------
// ------------------------------ Core Physics Update ------------------------
// ---------------------------------------------------------------------------

// Updates a single body's position based on its velocity and acceleration for a given time step
void gama_body_update_dt(gama_body *body, double dt) {
  if (body == NULL || !body->is_active || body->mass == 0) {
    return; // Don't update inactive or static bodies
  }
  // Apply acceleration to velocity
  body->velocity.x += body->acceleration.x * gama_dt;
  body->velocity.y += body->acceleration.y * gama_dt;
  // Apply velocity to position
  body->position.x += body->velocity.x * gama_dt;
  body->position.y += body->velocity.y * gama_dt;
}

void gama_body_update(gama_body *body) {
    return gama_body_update_dt(body, gama_dt);

}
// The main physics system function, now with sub-stepping
void gama_physics_update_ptr(gama_body **bodies, int count) {
  const int substeps = 5; // Number of sub-steps to perform
  const double sub_dt = gama_dt / substeps;

  for (int i = 0; i < substeps; i++) {
    // 1. Update all positions by a small amount
    for (int j = 0; j < count; j++) {
      gama_body_update_dt(bodies[j], sub_dt);
    }

    // 2. Check for and resolve all collisions
    for (int j = 0; j < count; ++j) {
      for (int k = j + 1; k < count; ++k) {
        if (!bodies[j]->is_active || !bodies[k]->is_active) {
          continue;
        }
        if (gama_collision_detect(bodies[j], bodies[k])) {
          gama_collision_resolve(bodies[j], bodies[k]);
        }
      }
    }
  }
}
// ---------------------------------------------------------------------------
// ----------------------------- Collision Detection -------------------------
// ---------------------------------------------------------------------------

// Helper for AABB (Axis-Aligned Bounding Box) collision
static inline int gama_aabb_vs_aabb(gama_body *a, gama_body *b) {
  double a_left = a->position.x - a->width / 2;
  double a_right = a->position.x + a->width / 2;
  double a_top = a->position.y + a->height / 2;
  double a_bottom = a->position.y - a->height / 2;

  double b_left = b->position.x - b->width / 2;
  double b_right = b->position.x + b->width / 2;
  double b_top = b->position.y + b->height / 2;
  double b_bottom = b->position.y - b->height / 2;

  // Check for no overlap and return early
  if (a_right < b_left || a_left > b_right || a_top < b_bottom ||
      a_bottom > b_top) {
    return 0;
  }
  return 1;
}

// Helper for Circle vs Circle collision
static inline int gama_circle_vs_circle(gama_body *a, gama_body *b) {
  double dx = b->position.x - a->position.x;
  double dy = b->position.y - a->position.y;
  double distance_sq = dx * dx + dy * dy;
  double total_radius = a->radius + b->radius;
  return distance_sq < (total_radius * total_radius);
}

// Helper for Circle vs AABB collision
static inline int gama_circle_vs_aabb(gama_body *circle, gama_body *rect) {
  double closest_x =
      fmax(rect->position.x - rect->width / 2,
           fmin(circle->position.x, rect->position.x + rect->width / 2));
  double closest_y =
      fmax(rect->position.y - rect->height / 2,
           fmin(circle->position.y, rect->position.y + rect->height / 2));

  double dx = circle->position.x - closest_x;
  double dy = circle->position.y - closest_y;

  return (dx * dx + dy * dy) < (circle->radius * circle->radius);
}

// Main collision detection dispatcher
int gama_collision_detect(gama_body *a, gama_body *b) {
  if (a->collider_type == GAMA_COLLIDER_RECT &&
      b->collider_type == GAMA_COLLIDER_RECT) {
    return gama_aabb_vs_aabb(a, b);
  }
  if (a->collider_type == GAMA_COLLIDER_CIRCLE &&
      b->collider_type == GAMA_COLLIDER_CIRCLE) {
    return gama_circle_vs_circle(a, b);
  }
  if (a->collider_type == GAMA_COLLIDER_CIRCLE &&
      b->collider_type == GAMA_COLLIDER_RECT) {
    return gama_circle_vs_aabb(a, b);
  }
  if (a->collider_type == GAMA_COLLIDER_RECT &&
      b->collider_type == GAMA_COLLIDER_CIRCLE) {
    return gama_circle_vs_aabb(b, a);
  }
  return 0; // No collision for other combinations
}

// ---------------------------------------------------------------------------
// ------------------------------ Collision Response -------------------------
// ---------------------------------------------------------------------------

void gama_collision_resolve(gama_body *a, gama_body *b) {
    double normal_x = 0, normal_y = 0, penetration_depth = 0;

    // --- 1. Calculate Penetration and Collision Normal ---
    if (a->collider_type == GAMA_COLLIDER_CIRCLE && b->collider_type == GAMA_COLLIDER_CIRCLE) {
        double dx = b->position.x - a->position.x;
        double dy = b->position.y - a->position.y;
        double distance = sqrt(dx * dx + dy * dy);
        if (distance == 0) {
             distance = 0.001; dx = 0.001; // Avoid division by zero
        }
        penetration_depth = a->radius + b->radius - distance;
        if (penetration_depth > 0) {
            normal_x = dx / distance;
            normal_y = dy / distance;
        }
    } else if (a->collider_type == GAMA_COLLIDER_RECT && b->collider_type == GAMA_COLLIDER_RECT) {
        double dx = b->position.x - a->position.x;
        double overlap_x = (a->width / 2 + b->width / 2) - fabs(dx);
        if (overlap_x > 0) {
            double dy = b->position.y - a->position.y;
            double overlap_y = (a->height / 2 + b->height / 2) - fabs(dy);
            if (overlap_y > 0) {
                if (overlap_x < overlap_y) {
                    penetration_depth = overlap_x;
                    normal_x = (dx < 0) ? -1 : 1;
                    normal_y = 0;
                } else {
                    penetration_depth = overlap_y;
                    normal_x = 0;
                    normal_y = (dy < 0) ? -1 : 1;
                }
            }
        }
    } else { // Circle vs Rectangle
        gama_body *circle = (a->collider_type == GAMA_COLLIDER_CIRCLE) ? a : b;
        gama_body *rect = (a->collider_type == GAMA_COLLIDER_RECT) ? a : b;

        double closest_x = fmax(rect->position.x - rect->width / 2, fmin(circle->position.x, rect->position.x + rect->width / 2));
        double closest_y = fmax(rect->position.y - rect->height / 2, fmin(circle->position.y, rect->position.y + rect->height / 2));

        double dx = circle->position.x - closest_x;
        double dy = circle->position.y - closest_y;
        double distance_sq = dx * dx + dy * dy;

        if (distance_sq < (circle->radius * circle->radius)) {
            double distance = sqrt(distance_sq);
            penetration_depth = circle->radius - distance;
            if (distance > 0) {
                normal_x = dx / distance;
                normal_y = dy / distance;
            } else { // Circle center is inside rect
                // Find axis of least penetration to push out
                double overlap_x = (rect->width/2 + circle->radius) - fabs(b->position.x - a->position.x);
                double overlap_y = (rect->height/2 + circle->radius) - fabs(b->position.y - a->position.y);
                 if (overlap_x < overlap_y) {
                    normal_x = (b->position.x - a->position.x < 0) ? -1 : 1;
                    normal_y = 0;
                } else {
                    normal_x = 0;
                    normal_y = (b->position.y - a->position.y < 0) ? -1 : 1;
                }
            }
            if (a != circle) { // Ensure normal always points from A to B
                normal_x = -normal_x;
                normal_y = -normal_y;
            }
        }
    }

    if (penetration_depth <= 0) {
        return; // No collision to resolve
    }

    // --- 2. Resolve Velocity ---
    double rel_vx = b->velocity.x - a->velocity.x;
    double rel_vy = b->velocity.y - a->velocity.y;
    double vel_along_normal = rel_vx * normal_x + rel_vy * normal_y;

    if (vel_along_normal > 0) {
        return;
    }

    double e = fmin(a->restitution, b->restitution);
    double j = -(1 + e) * vel_along_normal;

    double inv_mass_a = (a->mass > 0) ? 1.0 / a->mass : 0;
    double inv_mass_b = (b->mass > 0) ? 1.0 / b->mass : 0;
    if (inv_mass_a + inv_mass_b == 0) return;
    j /= (inv_mass_a + inv_mass_b);

    double impulse_x = j * normal_x;
    double impulse_y = j * normal_y;

    if (a->mass > 0) {
        a->velocity.x -= inv_mass_a * impulse_x;
        a->velocity.y -= inv_mass_a * impulse_y;
    }
    if (b->mass > 0) {
        b->velocity.x += inv_mass_b * impulse_x;
        b->velocity.y += inv_mass_b * impulse_y;
    }

    // --- 3. Positional Correction ---
    const double percent = 0.4;
    const double slop = 0.01;
    double correction_amount = fmax(penetration_depth - slop, 0.0) / (inv_mass_a + inv_mass_b) * percent;
    double correction_x = correction_amount * normal_x;
    double correction_y = correction_amount * normal_y;

    if (a->mass > 0) {
        a->position.x -= inv_mass_a * correction_x;
        a->position.y -= inv_mass_a * correction_y;
    }
    if (b->mass > 0) {
        b->position.x += inv_mass_b * correction_x;
        b->position.y += inv_mass_b * correction_y;
    }
}

void gama_physics_update(gama_body *bodies, int count) {
  gama_body **list = malloc(count * sizeof(gama_body *));
  for (size_t i = 0; i < count; i++)
    list[i] = &bodies[i];
  gama_physics_update_ptr(list, count);
}

void gama_physics_update2(gama_body *b1, gama_body *b2) {
  gama_body *bodies[] = {b1, b2};
  gama_physics_update_ptr(bodies, 2);
}

void gama_physics_update3(gama_body *b1, gama_body *b2, gama_body *b3) {
  gama_body *bodies[] = {b1, b2, b3};
  gama_physics_update_ptr(bodies, 3);
}

void gama_physics_update4(gama_body *b1, gama_body *b2, gama_body *b3,
                          gama_body *b4) {
  gama_body *bodies[] = {b1, b2, b3, b4};
  gama_physics_update_ptr(bodies, 4);
}

void gama_physics_update5(gama_body *b1, gama_body *b2, gama_body *b3,
                          gama_body *b4, gama_body *b5) {
  gama_body *bodies[] = {b1, b2, b3, b4, b5};
  gama_physics_update_ptr(bodies, 5);
}
