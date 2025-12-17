#pragma once

#include "body.h"
#include "system.h"
#include <math.h>

// ---------------------------------------------------------------------------
// ----------------------------- Collision Detection -------------------------
// ---------------------------------------------------------------------------

static inline int gm_aabb_vs_aabb(gmBody *a, gmBody *b) {
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
static inline int gm_circle_vs_circle(gmBody *a, gmBody *b) {
  double dx = b->position.x - a->position.x;
  double dy = b->position.y - a->position.y;
  double distance_sq = dx * dx + dy * dy;
  double total_radius = a->radius + b->radius;
  return distance_sq < (total_radius * total_radius);
}
// Accurate Circle vs Axis-Aligned Bounding Box collision
static inline int gm_circle_vs_aabb(const gmBody *circle, const gmBody *rect) {
  double half_w = rect->width * 0.5;
  double half_h = rect->height * 0.5;

  // Clamp circle center to rectangle bounds
  double closest_x = fmax(rect->position.x - half_w,
                          fmin(circle->position.x, rect->position.x + half_w));

  double closest_y = fmax(rect->position.y - half_h,
                          fmin(circle->position.y, rect->position.y + half_h));

  // Vector from closest point to circle center
  double dx = circle->position.x - closest_x;
  double dy = circle->position.y - closest_y;

  // Check collision (<= catches "touching" cases)
  return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

// Main collision detection dispatcher
gmCollision *gm_collision_detect(gmBody *a, gmBody *b) {
  int collided = 0;
  if (a->collider_type == GM_COLLIDER_RECT &&
      b->collider_type == GM_COLLIDER_RECT) {
    collided = gm_aabb_vs_aabb(a, b);
  }
  if (a->collider_type == GM_COLLIDER_CIRCLE &&
      b->collider_type == GM_COLLIDER_CIRCLE) {
    collided = gm_circle_vs_circle(a, b);
  }
  if (a->collider_type == GM_COLLIDER_CIRCLE &&
      b->collider_type == GM_COLLIDER_RECT) {
    collided = gm_circle_vs_aabb(a, b);
  }
  if (a->collider_type == GM_COLLIDER_RECT &&
      b->collider_type == GM_COLLIDER_CIRCLE) {
    collided = gm_circle_vs_aabb(b, a);
  }
  if (!collided)
    return NULL; // No collision for other combinations
  gmCollision *collision = malloc(sizeof(gmCollision));
  collision->bodies[0] = a;
  collision->bodies[1] = b;
  collision->normals = gmpos(0, 0);
  collision->penetration = 0;
  collision->since = 0;
  collision->sys = NULL;
  return collision;
}

int gm_body_contains(gmBody *body, double x, double y) {
  double dx = fabs(body->position.x - x);
  double dy = fabs(body->position.y - y);
  switch (body->collider_type) {
  case GM_COLLIDER_RECT:
    return (dx < body->width * 0.5) && (dy < body->height * 0.5);
  case GM_COLLIDER_CIRCLE:
    return (dx * dx) + (dy * dy) < (body->radius * body->radius);
  default:
    return 0;
  }
}

int gm_mouse_in_rect(const double x, const double y, const double w,
                     const double h) {
  return pow(gm_mouse.position.x - x, 2) < pow(w / 2, 2) &&
         pow(gm_mouse.position.y - y, 2) < pow(h / 2, 2);
}
int gm_mouse_in_circle(const double x, const double y, const double r) {
  return pow(gm_mouse.position.x - x, 2) + pow(gm_mouse.position.y, 2) <
         pow(r, 2);
}
