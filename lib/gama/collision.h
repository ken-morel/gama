/**
 * @file collision.h
 * @brief Defines collision structures and provides functions for 2D collision detection.
 *
 * This file contains the core logic for detecting collisions between different
 * types of physics bodies (rectangles and circles) and structures to hold
 * collision information.
 */
#pragma once

#include "body.h"
#include "system.h" // For gmSystem
#include <math.h>

/**
 * @brief Structure to store detailed information about a collision between two bodies.
 */
typedef struct gm_collision {
  gmBody *bodies[2]; /**< Pointers to the two bodies involved in the collision. */
  gmPos normals;     /**< The normal vector of the collision, pointing from bodies[0] to bodies[1]. */
  double penetration; /**< The penetration depth of the collision (how much bodies overlap). */
  double since;       /**< The total time the bodies have been in collision (for continuous collision detection). */
  gmSystem *sys;     /**< Pointer to the physics system managing this collision (can be NULL). */
} gmCollision;


// ---------------------------------------------------------------------------
// ----------------------------- Collision Detection -------------------------
// ---------------------------------------------------------------------------

/**
 * @internal
 * @brief Checks for collision between two Axis-Aligned Bounding Boxes (AABB).
 * @param a Pointer to the first rectangular body.
 * @param b Pointer to the second rectangular body.
 * @return 1 if the AABBs overlap, 0 otherwise.
 */
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

/**
 * @internal
 * @brief Checks for collision between two circles.
 * @param a Pointer to the first circular body.
 * @param b Pointer to the second circular body.
 * @return 1 if the circles overlap, 0 otherwise.
 */
static inline int gm_circle_vs_circle(gmBody *a, gmBody *b) {
  double dx = b->position.x - a->position.x;
  double dy = b->position.y - a->position.y;
  double distance_sq = dx * dx + dy * dy;
  double total_radius = a->radius + b->radius;
  return distance_sq < (total_radius * total_radius);
}
/**
 * @internal
 * @brief Checks for collision between a circle and an Axis-Aligned Bounding Box (AABB).
 * @param circle Pointer to the circular body.
 * @param rect Pointer to the rectangular body.
 * @return 1 if the circle and rectangle overlap, 0 otherwise.
 */
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
/**
 * @brief Detects a collision between two physics bodies based on their collider types.
 *
 * This function dispatches to specific collision tests (e.g., AABB vs AABB,
 * Circle vs Circle, Circle vs AABB) based on the `collider_type` of the input bodies.
 *
 * @param a Pointer to the first body.
 * @param b Pointer to the second body.
 * @return A dynamically allocated `gmCollision` structure if a collision occurs,
 *         otherwise NULL. The caller is responsible for freeing the returned
 *         `gmCollision` object if it's not managed by a `gmSystem`.
 */
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
  collision->normals = gmpos(0, 0); // Initialize to zero
  collision->penetration = 0;      // Initialize to zero
  collision->since = 0;
  collision->sys = NULL;
  return collision;
}

/**
 * @brief Checks if a given point (x, y) is contained within a body's collider.
 *
 * This function performs a point-in-rectangle test for `GM_COLLIDER_RECT`
 * and a point-in-circle test for `GM_COLLIDER_CIRCLE`.
 *
 * @param body Pointer to the body to check.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return 1 if the point is inside the body's collider, 0 otherwise.
 */
int gm_body_contains(gmBody *body, double x, double y) {
  double dx = fabs(body->position.x - x);
  double dy = fabs(body->position.y - y);
  switch (body->collider_type) {
  case GM_COLLIDER_RECT:
    return (dx < body->width * 0.5) && (dy < body->height * 0.5);
  case GM_COLLIDER_CIRCLE:
    return (dx * dx) + (dy * dy) < (body->radius * body->radius);
  default:
    return 0; // Should not happen with current collider types
  }
}

/**
 * @brief Checks if the mouse cursor is currently within a given rectangular area.
 * @param x The x-coordinate of the center of the rectangle.
 * @param y The y-coordinate of the center of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @return 1 if the mouse is inside the rectangle, 0 otherwise.
 */
int gm_mouse_in_rect(const double x, const double y, const double w,
                     const double h) {
  return fabs(gm_mouse.position.x - x) < w / 2 &&
         fabs(gm_mouse.position.y - y) < h / 2;
}
/**
 * @brief Checks if the mouse cursor is currently within a given circular area.
 * @param x The x-coordinate of the center of the circle.
 * @param y The y-coordinate of the center of the circle.
 * @param r The radius of the circle.
 * @return 1 if the mouse is inside the circle, 0 otherwise.
 */
int gm_mouse_in_circle(const double x, const double y, const double r) {
  return pow(gm_mouse.position.x - x, 2) + pow(gm_mouse.position.y - y, 2) <
         pow(r, 2);
}
