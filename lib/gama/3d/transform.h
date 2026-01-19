/**
 * @file transform.h
 * @brief Defines structures and functions for 3D transformations (position, rotation, scale).
 *
 * This file provides the `gm3Transform` structure to encapsulate the
 * translation, rotation, and scaling of 3D objects, along with helper
 * functions to apply these transformations to 3D points and vectors.
 */
 #ifndef GM3_TRANSFORM_H_INCLUDED
 #define GM3_TRANSFORM_H_INCLUDED

#include "position.h"

/**
 * @brief Represents a 3D transformation, including position, rotation, and scale.
 */
typedef struct {
  gm3Pos position; /**< Translation vector. */
  gm3Pos rotation; /**< Rotation vector (Euler angles in radians, X, Y, Z). */
  gm3Pos scale;    /**< Scaling vector. */
} gm3Transform;

/**
 * @brief A default `gm3Transform` instance.
 *
 * Initializes a transform with default position `{0, 0, 20}`, no rotation,
 * and uniform scale `{1, 1, 1}`.
 */
const gm3Transform gm3_default_transform = {
    .position = {0, 0, 20}, .rotation = {0}, .scale = {1, 1, 1}};

/**
 * @brief Rotates a `gm3Pos` vector by the given Euler angles (X, Y, Z).
 *
 * The rotation is applied sequentially around the X, then Y, then Z axes.
 *
 * @param res A pointer to the `gm3Pos` vector to rotate (modified in place).
 * @param rot A pointer to a `gm3Pos` containing the Euler angles (radians) for rotation around X, Y, and Z axes.
 */
void gm3_pos_rotate(gm3Pos *res, const gm3Pos *rot) {
  if (!res || !rot)
    return;
  double temp;

  // 1. Rotate around X-axis
  temp = res->y * cos(rot->x) - res->z * sin(rot->x);
  res->z = res->y * sin(rot->x) + res->z * cos(rot->x);
  res->y = temp;

  // 2. Rotate around Y-axis
  temp = res->x * cos(rot->y) + res->z * sin(rot->y);
  res->z = -res->x * sin(rot->y) + res->z * cos(rot->y);
  res->x = temp;

  // 3. Rotate around Z-axis
  temp = res->x * cos(rot->z) - res->y * sin(rot->z);
  res->y = res->x * sin(rot->z) + res->y * cos(rot->z);
  res->x = temp;
}
/**
 * @brief Applies a `gm3Transform` to a `gm3Pos` vector.
 *
 * The transformation is applied in the order: scale, then rotate, then translate.
 *
 * @param p A pointer to the `gm3Pos` vector to transform (modified in place).
 * @param t A pointer to the `gm3Transform` to apply.
 */
void gm3_transform_pos(gm3Pos *p, const gm3Transform *t) {
  gm3_pos_mul(p, &t->scale);
  gm3_pos_rotate(p, &t->rotation);
  gm3_pos_add(p, &t->position);
}

/**
 * @brief Creates a new `gm3Transform` struct initialized to identity.
 *
 * The position and rotation are set to zero, and scale is set to one.
 *
 * @return A new identity `gm3Transform` instance.
 */
gm3Transform gm3_transform() {
  gm3Transform t;
  gm3_pos_reset(&t.position);
  gm3_pos_reset(&t.rotation);
  t.scale.x = 1;
  t.scale.y = 1;
  t.scale.z = 1;
  return t;
}
#endif // GM3_TRANSFORM_H_INCLUDED
