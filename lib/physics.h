#pragma once

#include "body.h"
#include "position.h"
#include <math.h>

// --- Forward declarations for collision functions ---
int gama_collision_detect(gama_body *a, gama_body *b);
void gama_collision_resolve(gama_body *a, gama_body *b);


// ---------------------------------------------------------------------------
// ------------------------------ Core Physics Update ------------------------
// ---------------------------------------------------------------------------

// Updates a single body's position based on its velocity and acceleration
void gama_body_update(gama_body *body, double dt) {
  if (body == NULL || !body->is_active || body->mass == 0) {
    return; // Don't update inactive or static bodies
  }

  // Apply acceleration to velocity
  body->velocity.x += body->acceleration.x * dt;
  body->velocity.y += body->acceleration.y * dt;

  // Apply velocity to position
  body->position.x += body->velocity.x * dt;
  body->position.y += body->velocity.y * dt;
}

// The main physics system function
void gama_physics_update(gama_body bodies[], int count, double dt) {
  // 1. Update all positions
  for (int i = 0; i < count; ++i) {
    gama_body_update(&bodies[i], dt);
  }

  // 2. Check for and resolve collisions (simple N-body check)
  for (int i = 0; i < count; ++i) {
    for (int j = i + 1; j < count; ++j) {
      if (!bodies[i].is_active || !bodies[j].is_active) {
        continue;
      }
      if (gama_collision_detect(&bodies[i], &bodies[j])) {
        gama_collision_resolve(&bodies[i], &bodies[j]);
      }
    }
  }
}

// ---------------------------------------------------------------------------
// ----------------------------- Collision Detection -------------------------
// ---------------------------------------------------------------------------

// Helper for AABB (Axis-Aligned Bounding Box) collision
int aabb_vs_aabb(gama_body *a, gama_body *b) {
    double a_left = a->position.x - a->width / 2;
    double a_right = a->position.x + a->width / 2;
    double a_top = a->position.y + a->height / 2;
    double a_bottom = a->position.y - a->height / 2;

    double b_left = b->position.x - b->width / 2;
    double b_right = b->position.x + b->width / 2;
    double b_top = b->position.y + b->height / 2;
    double b_bottom = b->position.y - b->height / 2;

    // Check for no overlap and return early
    if (a_right < b_left || a_left > b_right || a_top < b_bottom || a_bottom > b_top) {
        return 0;
    }
    return 1;
}

// Helper for Circle vs Circle collision
int circle_vs_circle(gama_body *a, gama_body *b) {
    double dx = b->position.x - a->position.x;
    double dy = b->position.y - a->position.y;
    double distance_sq = dx * dx + dy * dy;
    double total_radius = a->radius + b->radius;
    return distance_sq < (total_radius * total_radius);
}

// Helper for Circle vs AABB collision
int circle_vs_aabb(gama_body *circle, gama_body *rect) {
    double closest_x = fmax(rect->position.x - rect->width / 2, fmin(circle->position.x, rect->position.x + rect->width / 2));
    double closest_y = fmax(rect->position.y - rect->height / 2, fmin(circle->position.y, rect->position.y + rect->height / 2));

    double dx = circle->position.x - closest_x;
    double dy = circle->position.y - closest_y;

    return (dx * dx + dy * dy) < (circle->radius * circle->radius);
}


// Main collision detection dispatcher
int gama_collision_detect(gama_body *a, gama_body *b) {
    if (a->collider_type == GAMA_COLLIDER_RECT && b->collider_type == GAMA_COLLIDER_RECT) {
        return aabb_vs_aabb(a, b);
    }
    if (a->collider_type == GAMA_COLLIDER_CIRCLE && b->collider_type == GAMA_COLLIDER_CIRCLE) {
        return circle_vs_circle(a, b);
    }
    if (a->collider_type == GAMA_COLLIDER_CIRCLE && b->collider_type == GAMA_COLLIDER_RECT) {
        return circle_vs_aabb(a, b);
    }
    if (a->collider_type == GAMA_COLLIDER_RECT && b->collider_type == GAMA_COLLIDER_CIRCLE) {
        return circle_vs_aabb(b, a);
    }
    return 0; // No collision for other combinations
}


// ---------------------------------------------------------------------------
// ------------------------------ Collision Response -------------------------
// ---------------------------------------------------------------------------

void gama_collision_resolve(gama_body *a, gama_body *b) {
    double dx = b->position.x - a->position.x;
    double dy = b->position.y - a->position.y;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance == 0) { // Avoid division by zero; default to an upward normal
        dx = 0; dy = 1; distance = 1;
    }

    double normal_x = dx / distance;
    double normal_y = dy / distance;

    // Relative velocity
    double rel_vx = b->velocity.x - a->velocity.x;
    double rel_vy = b->velocity.y - a->velocity.y;

    double vel_along_normal = rel_vx * normal_x + rel_vy * normal_y;

    // Do not resolve if velocities are separating
    if (vel_along_normal > 0) {
        return;
    }

    // Use the minimum bounciness and friction
    double e = fmin(a->restitution, b->restitution); 
    double sf = fmin(a->friction, b->friction);
    double df = fmin(a->friction, b->friction);


    double j = -(1 + e) * vel_along_normal;
    double inv_mass_a = (a->mass > 0) ? 1.0 / a->mass : 0;
    double inv_mass_b = (b->mass > 0) ? 1.0 / b->mass : 0;
    if (inv_mass_a + inv_mass_b == 0) return; // Both are static
    j /= (inv_mass_a + inv_mass_b);

    // Apply impulse
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
    
    // Positional correction to prevent sinking
    const double percent = 0.4; // How much to correct by
    const double slop = 0.01;   // How much overlap to allow
    double penetration_depth;

    if (a->collider_type == GAMA_COLLIDER_RECT && b->collider_type == GAMA_COLLIDER_RECT) {
        penetration_depth = (a->width/2 + b->width/2) - fabs(dx);
    } else if (a->collider_type == GAMA_COLLIDER_CIRCLE && b->collider_type == GAMA_COLLIDER_CIRCLE) {
        penetration_depth = (a->radius + b->radius) - distance;
    } else { // Simplified for circle-rect
        penetration_depth = (a->collider_type == GAMA_COLLIDER_CIRCLE ? a->radius : a->width/2) + 
                            (b->collider_type == GAMA_COLLIDER_CIRCLE ? b->radius : b->width/2) - distance;
    }
    
    if (penetration_depth > slop) {
      double correction_amount = (penetration_depth - slop) / (inv_mass_a + inv_mass_b) * percent;
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
}