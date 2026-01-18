#ifndef GM_BODY_H_INCLUDED
#define GM_BODY_H_INCLUDED

#include "mouse.h"
#include <stdint.h>

/**
 * @brief Enum to define the type of collider for a physics body.
 */
typedef enum {
  GM_COLLIDER_CIRCLE, /**< Circular collider */
  GM_COLLIDER_RECT    /**< Rectangular collider */
} gmColliderType;

/**
 * @brief Structure representing a physics body with properties for collision
 * and movement.
 */
typedef struct {
  uint8_t
      is_active; /**< Whether the body is active in the physics simulation. Set to 0 to disable. */
  uint8_t is_static; /**< Whether the body is static (immovable). Static bodies do not move or respond to forces. */

  gmColliderType collider_type; /**< Type of collider (rectangle or circle). */
  gmPos position;               /**< Current position of the body. */
  gmPos velocity;               /**< Current velocity of the body. */
  gmPos acceleration;           /**< Current acceleration of the body. */

  double width, height, radius; /**< Dimensions of the body (width/height for
                                   rectangles, radius for circles). For circles,
                                   width and height are typically set to 2*radius. */
  double mass;                  /**< Mass of the body. Set to 0 for infinite mass (similar to static but can move). */
  double restitution; /**< Bounciness of the body (0.0 = no bounce, 1.0+ =
                         more bounce). */
  double friction; /**< Friction coefficient (not currently used in the code). */
} gmBody;

/**
 * @brief Creates a new physics body with specified properties.
 *
 * The `radius` for circular colliders is automatically set based on `w` (or `h` if smaller).
 * For rectangular colliders, `radius` is set to `w < h ? w : h`.
 *
 * @param mass The mass of the body. Set to 0 for infinite mass.
 * @param x The x-coordinate of the body's initial position.
 * @param y The y-coordinate of the body's initial position.
 * @param w The width of the body (or diameter for circles).
 * @param h The height of the body (or diameter for circles).
 * @param c The type of collider for the body (`GM_COLLIDER_RECT` or `GM_COLLIDER_CIRCLE`).
 * @return A new `gmBody` instance initialized with default active/non-static properties.
 */
gmBody gm_body_create(double mass, double x, double y, double w, double h,
                      gmColliderType c) {
  gmBody body = {
      .is_active = 1,
      .is_static = 0,
      .collider_type = c,
      .position = {.x = x, .y = y},
      .velocity = {.x = 0, .y = 0},
      .acceleration = {.x = 0, .y = 0},
      .width = w,
      .height = h,
      .radius = w < h ? w : h, // Radius is set from minimum of width/height
      .mass = mass,
      .restitution = 1,
  };
  return body;
}

/**
 * @brief Limits the maximum speed of a body.
 *
 * If the body's current speed exceeds `max_speed`, its velocity is scaled
 * down to match `max_speed` while preserving its direction.
 *
 * @param body Pointer to the body to modify.
 * @param max_speed The maximum allowed speed.
 */
void gm_max_speed(gmBody *body, double max_speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed > max_speed) {
    double factor = max_speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}

/**
 * @brief Sets the minimum speed of a body.
 *
 * If the body's current speed falls below `min_speed` (and is not zero),
 * its velocity is scaled up to match `min_speed` while preserving its direction.
 * If the body is stationary (speed is 0), its velocity remains 0.
 *
 * @param body Pointer to the body to modify.
 * @param min_speed The minimum allowed speed.
 */
void gm_min_speed(gmBody *body, double min_speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed < min_speed) {
    // Avoid division by zero if body is not moving.
    if (current_speed == 0)
      return;

    double factor = min_speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}

/**
 * @brief Sets the speed of a body while preserving its current direction.
 *
 * If the body is stationary, it starts moving along the X-axis with the
 * specified `speed`.
 *
 * @param body Pointer to the body to modify.
 * @param speed The target speed to set.
 */
void gm_speed(gmBody *body, double speed) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed == speed)
    return;

  if (current_speed == 0) {
    // If speed is 0, we have no direction. Assume movement along X axis.
    body->velocity.x = speed;
    body->velocity.y = 0;
  } else {
    double factor = speed / current_speed;
    body->velocity.x *= factor;
    body->velocity.y *= factor;
  }
}

/**
 * @brief Limits the maximum speed of a body using an animation function.
 *
 * This function applies a smooth animation to the body's velocity components
 * if its speed exceeds `max_speed`.
 *
 * @param body Pointer to the body to modify.
 * @param max_speed The maximum allowed speed.
 * @param animator Function pointer to animate the velocity change.
 * @param dt Delta time for animation.
 * @param t Time parameter for animation.
 */
void gm_max_speed_anim(gmBody *body, double max_speed,
                       void animator(double *value, double target, double dt,
                                     double t),
                       double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed > max_speed) {
    double factor = max_speed / current_speed;
    double x_target = body->velocity.x * factor;
    double y_target = body->velocity.y * factor;
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}

/**
 * @brief Sets the minimum speed of a body using an animation function.
 *
 * This function applies a smooth animation to the body's velocity components
 * if its speed falls below `min_speed` (and is not zero).
 *
 * @param body Pointer to the body to modify.
 * @param min_speed The minimum allowed speed.
 * @param animator Function pointer to animate the velocity change.
 * @param dt Delta time for animation.
 * @param t Time parameter for animation.
 */
void gm_min_speed_anim(gmBody *body, double min_speed,
                       void animator(double *value, double target, double dt,
                                     double t),
                       double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed < min_speed) {
    // Avoid division by zero if body is not moving.
    if (current_speed == 0)
      return;
    double factor = min_speed / current_speed;
    double x_target = body->velocity.x * factor;
    double y_target = body->velocity.y * factor;
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}

/**
 * @brief Sets the speed of a body using an animation function, preserving its direction.
 *
 * This function applies a smooth animation to the body's velocity components
 * to reach the `speed`. If the body is stationary, it starts moving along
 * the X-axis.
 *
 * @param body Pointer to the body to modify.
 * @param speed The target speed to set.
 * @param animator Function pointer to animate the velocity change.
 * @param dt Delta time for animation.
 * @param t Time parameter for animation.
 */
void gm_speed_anim(gmBody *body, double speed,
                   void animator(double *value, double target, double dt,
                                 double t),
                   double dt, double t) {
  double current_speed = gm_pos_magniture(body->velocity);
  if (current_speed != speed) {
    double x_target, y_target;
    if (current_speed == 0) {
      // If speed is 0, we have no direction. Assume movement along X axis.
      x_target = speed;
      y_target = 0;
    } else {
      double factor = speed / current_speed;
      x_target = body->velocity.x * factor;
      y_target = body->velocity.y * factor;
    }
    animator(&body->velocity.x, x_target, dt, t);
    animator(&body->velocity.y, y_target, dt, t);
  }
}

/**
 * @brief Creates a rectangular physics body.
 * @param m The mass of the body.
 * @param x The x-coordinate of the body's position.
 * @param y The y-coordinate of the body's position.
 * @param w The width of the body.
 * @param h The height of the body.
 * @return A new rectangular gmBody instance.
 */
gmBody gm_rectangle_body(double m, double x, double y, double w, double h) {
  return gm_body_create(m, x, y, w, h, GM_COLLIDER_RECT);
}

/**
 * @brief Creates a circular physics body.
 * @param m The mass of the body.
 * @param x The x-coordinate of the body's position.
 * @param y The y-coordinate of the body's position.
 * @param r The radius of the body.
 * @return A new circular gmBody instance.
 */
gmBody gm_circle_body(double m, double x, double y, double r) {
  return gm_body_create(m, x, y, r * 2, r * 2, GM_COLLIDER_CIRCLE); // Pass diameter for w/h
}

/**
 * @brief Checks if a point is contained within a body's collider.
 * @param body Pointer to the body to check.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return 1 if the point is inside the body, 0 otherwise.
 */
int gm_body_contains(gmBody *body, double x, double y);

/**
 * @brief Checks if a body is currently being hovered over by the mouse.
 * @param body Pointer to the body to check.
 * @return 1 if the body is being hovered over, 0 otherwise.
 */
static inline int gm_hovered(gmBody *body) {
  return gm_body_contains(body, gm_mouse.position.x, gm_mouse.position.y);
}

/**
 * @brief Checks if a body is currently being clicked by the mouse.
 * @param body Pointer to the body to check.
 * @return 1 if the body is being clicked, 0 otherwise.
 */
static inline int gm_clicked(gmBody *body) {
  return gm_mouse.clicked && gm_hovered(body);
}

/**
 * @brief Constrains a body within specified rectangular boundaries by clipping its
 * position.
 *
 * If a boundary parameter (bx, ex, by, ey) is 0, that boundary is ignored.
 * The body's position is clamped to remain within the specified range.
 *
 * @param body Pointer to the body to constrain.
 * @param bx The beginning x-coordinate of the boundary. If 0, the left boundary is ignored.
 * @param ex The ending x-coordinate of the boundary. If 0, the right boundary is ignored.
 * @param by The beginning y-coordinate of the boundary. If 0, the bottom boundary is ignored.
 * @param ey The ending y-coordinate of the boundary. If 0, the top boundary is ignored.
 * @return A bitmask indicating which boundaries were exceeded and clipped:
 *         - Bit 3 (0b1000): Left boundary (`body->position.x < bx`)
 *         - Bit 2 (0b0100): Right boundary (`body->position.x > ex`)
 *         - Bit 1 (0b0010): Bottom boundary (`body->position.y < by`)
 *         - Bit 0 (0b0001): Top boundary (`body->position.y > ey`)
 */
static inline uint8_t gm_body_bound_clip(gmBody *body, double bx, double ex,
                                         double by, double ey) {

  int8_t exited = 0;
  if (bx != 0 || ex != 0) { // Only apply if bounds are specified
    if (body->position.x < bx) {
      body->position.x = bx;
      exited |= 0b1000;
    } else if (body->position.x > ex) {
      body->position.x = ex;
      exited |= 0b0100;
    }
  }
  if (by != 0 || ey != 0) { // Only apply if bounds are specified
    if (body->position.y < by) {
      body->position.y = by;
      exited |= 0b0010;
    } else if (body->position.y > ey) {
      body->position.y = ey;
      exited |= 0b0001;
    }
  }
  return exited;
}

/**
 * @brief Wraps a body around specified rectangular boundaries (like a torus).
 *
 * If the body exits one side, it reappears on the opposite side.
 * Note: If a boundary parameter is 0, that boundary check is skipped.
 *
 * @param body Pointer to the body to wrap.
 * @param bx The beginning x-coordinate of the boundary.
 * @param ex The ending x-coordinate of the boundary.
 * @param by The beginning y-coordinate of the boundary.
 * @param ey The ending y-coordinate of the boundary.
 * @return A bitmask indicating which boundaries were exceeded:
 *         - Bit 3 (0b1000): Left boundary (`body->position.x < bx`)
 *         - Bit 2 (0b0100): Right boundary (`body->position.x > ex`)
 *         - Bit 1 (0b0010): Bottom boundary (`body->position.y < by`)
 *         - Bit 0 (0b0001): Top boundary (`body->position.y > ey`)
 */
static inline int8_t gm_body_bound_reflect(gmBody *body, double bx, double ex,
                                           double by, double ey) {
  int8_t exited = 0;
  if (body->position.x < bx) {
    body->position.x = ex;
    exited |= 0b1000;
  } else if (body->position.x > ex) {
    body->position.x = bx;
    exited |= 0b0100;
  }
  if (body->position.y < by) {
    body->position.y = ey;
    exited |= 0b0010;
  } else if (body->position.y > ey) {
    body->position.y = by;
    exited |= 0b0001;
  }
  return exited;
}

/**
 * @brief Bounces a body when it exceeds specified rectangular boundaries.
 *
 * If the body's position goes beyond a boundary and its velocity is moving
 * further out of bounds, its velocity component in that direction is reversed
 * and scaled by the `restitution` factor.
 *
 * @param body Pointer to the body to bounce.
 * @param bx The beginning x-coordinate of the boundary.
 * @param ex The ending x-coordinate of the boundary.
 * @param by The beginning y-coordinate of the boundary.
 * @param ey The ending y-coordinate of the boundary.
 * @param restitution The bounciness factor to apply (0.0 for no bounce, 1.0 for perfect bounce).
 * @return A bitmask indicating which boundaries were exceeded:
 *         - Bit 3 (0b1000): Left boundary (`body->position.x < bx`)
 *         - Bit 2 (0b0100): Right boundary (`body->position.x > ex`)
 *         - Bit 1 (0b0010): Bottom boundary (`body->position.y < by`)
 *         - Bit 0 (0b0001): Top boundary (`body->position.y > ey`)
 */
static inline int8_t gm_body_bound_bounce(gmBody *body, double bx, double ex,
                                          double by, double ey,
                                          double restitution) {
  int8_t exited = 0;
  if (body->position.x < bx && body->velocity.x < 0) {
    body->position.x = bx; // Snap to boundary to prevent sinking
    body->velocity.x = -body->velocity.x * restitution;
    exited |= 0b1000;
  } else if (body->position.x > ex && body->velocity.x > 0) {
    body->position.x = ex; // Snap to boundary
    body->velocity.x = -body->velocity.x * restitution;
    exited |= 0b0100;
  }
  if (body->position.y < by && body->velocity.y < 0) {
    body->position.y = by; // Snap to boundary
    body->velocity.y = -body->velocity.y * restitution;
    exited |= 0b0010;
  } else if (body->position.y > ey && body->velocity.y > 0) {
    body->position.y = ey; // Snap to boundary
    body->velocity.y = -body->velocity.y * restitution;
    exited |= 0b0001;
  }
  return exited;
}
#endif
