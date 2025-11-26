#pragma once

#include "body.h"
#include "gapi.h"
#include "position.h"
#include "system.h"
#include <math.h>
#include <stdio.h>

int gm_collision_detect(gmBody *a, gmBody *b);
void gm_collision_resolve(gmSystem *, gmBody *a, gmBody *b);

// ---------------------------------------------------------------------------
// ------------------------------ Core Physics Update ------------------------
// ---------------------------------------------------------------------------

void gm_system_update_body_dt(gmSystem *sys, gmBody *body, double dt) {

  if (body == NULL || (sys != NULL && !sys->is_active) || !body->is_active) {
    return; // Don't update inactive or static bodies
  }
  if (body->mass == 0 && sys != NULL) { // for static bodies
    body->position.x +=
        sys->velocity.x * dt; // Update position with system velocity
    body->position.y += sys->velocity.y * dt;
  } else {
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
}

void gm_system_update_dt(gmSystem *sys, unsigned substeps, double dt) {
  if (sys == NULL)
    return;
  if (!sys->is_active)
    return;

  const double sub_dt = gm_dt() / substeps;
  const unsigned count = gm_system_size(sys);

  for (int i = 0; i < substeps; i++) {
    for (int j = 0; j < count; j++) {
      gm_system_update_body_dt(sys, sys->bodies[j], sub_dt);
    }

    for (int j = 0; j < count; j++) {
      for (int k = j + 1; k < count; k++) {
        if (!sys->bodies[j]->is_active || !sys->bodies[k]->is_active) {
          continue;
        }
        // Note: We usually skip if both masses are 0 (static vs static),
        // but checking it inside resolve is safer if you want triggers.
        if (sys->bodies[j]->mass == 0 && sys->bodies[k]->mass == 0) {
          continue;
        }

        if (gm_collision_detect(sys->bodies[j], sys->bodies[k])) {
          gm_collision_resolve(sys, sys->bodies[j], sys->bodies[k]);
        }
      }
    }
  }
}

static inline void gm_system_update(gmSystem *sys, unsigned substeps) {
  return gm_system_update_dt(sys, substeps, gm_dt());
}

// ---------------------------------------------------------------------------
// ------------------------------ Collision Response -------------------------
// ---------------------------------------------------------------------------


double gm_collision_penetration_normals(gmBody *a, gmBody *b, double * normal_x, double *normal_y) {
  double penetration_depth;
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
    if (penetration_depth > 0 && normal_x !=NULL && normal_y != NULL) {
      *normal_x = dx / distance;
      *normal_y = dy / distance;
    }
  }
  // CASE: Rect vs Rect
  else if (a->collider_type == GM_COLLIDER_RECT &&
           b->collider_type == GM_COLLIDER_RECT) {
    double dx = b->position.x - a->position.x;
    double overlap_x = (a->width / 2 + b->width / 2) - fabs(dx);
    if (overlap_x > 0) {
      double dy = b->position.y - a->position.y;
      double overlap_y = (a->height / 2 + b->height / 2) - fabs(dy);
      if (overlap_y > 0) {
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
    }
  }
  // CASE: Circle vs Rectangle
  else {
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
      // Sub-case B: Center is INSIDE the rectangle
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
        if(normal_x != NULL && normal_y != NULL) {
        if (min_x < min_y) {
          penetration_depth = min_x + circle->radius;
          *normal_x = (left_pen < right_pen) ? -1 : 1;
          *normal_y = 0;
        } else {
          penetration_depth = min_y + circle->radius;
          *normal_x = 0;
          *normal_y = (bottom_pen < top_pen) ? -1 : 1;
        }}
      }

      // --- CRITICAL FIX ---
      // The logic above calculates a normal pointing Rect -> Circle.
      // We must ensure the final 'normal' used for resolution points from A ->
      // B.
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

double gm_collision_penetration(gmBody *a, gmBody *b) {
  return gm_collision_penetration_normals(a, a, NULL, NULL);

}
void gm_collision_resolve(gmSystem *sys, gmBody *a, gmBody *b) {
  double normal_x, normal_y;
  double penetration_depth = gm_collision_penetration_normals(a, b, &normal_x, &normal_y);

  if (penetration_depth <= 0) {
    return; // No collision to resolve
  }

  // --- 2. Resolve Velocity ---
  double rel_vx = b->velocity.x - a->velocity.x;
  double rel_vy = b->velocity.y - a->velocity.y;
  double vel_along_normal = rel_vx * normal_x + rel_vy * normal_y;

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
  const double percent = 0.2; // Percentage of penetration to correct
  const double slop = 0.01;   // Penetration allowance to prevent jitter
  double correction_amount =
      fmax(penetration_depth - slop, 0.0) / (inv_mass_a + inv_mass_b) * percent;

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


