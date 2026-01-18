#ifndef GM_POSITION_H_INCLUDED
#define GM_POSITION_H_INCLUDED
#include "../_math.h"
#include <stddef.h>
#include <string.h>

#include "../str.h"
/**
 * @brief Represents a 3D position or vector.
 */
typedef struct {
  double x, y, z; /**< The X, Y, and Z coordinates. */
} gm3Pos;

const gm3Pos gm3zero = {0};

/**
 * @brief Creates a new `gm3Pos` struct with the given coordinates.
 * @param x The X-coordinate.
 * @param y The Y-coordinate.
 * @param z The Z-coordinate.
 * @return A new `gm3Pos` instance.
 */
#define gm3pos(x, y, z) ((gm3Pos){x, y, z})

/**
 * @brief Sets the coordinates of an existing `gm3Pos` struct.
 * @param p A pointer to the `gm3Pos` struct to modify.
 * @param x The new X-coordinate.
 * @param y The new Y-coordinate.
 * @param z The new Z-coordinate.
 */
static inline void gm3_pos_set(gm3Pos *p, double x, double y, double z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

/**
 * @brief Calculates the magnitude (length) of a `gm3Pos` vector.
 * @param p The `gm3Pos` vector.
 * @return The magnitude of the vector.
 */
#define gm3_pos_magnitude(p) sqrt((p).x *(p).x + (p).y * (p).y + (p).z * (p).z)

/**
 * @brief Calculates the Euclidean distance between two `gm3Pos` points.
 * @param a The first `gm3Pos` point.
 * @param b The second `gm3Pos` point.
 * @return The distance between the two points.
 */
#define gm3_pos_distance(a, b)                                                 \
  sqrt(pow((a).x - (b).x, 2) + pow((a).y - (b).y, 2) + pow((a).z - (b).z, 2))

/**
 * @brief Creates a `gm3Pos` from a 2D `gmPos` and a Z-coordinate.
 * @param p The 2D `gmPos`.
 * @param z The Z-coordinate.
 * @return A new `gm3Pos` instance.
 */
#define gm3_pos_from2(p, z) gm3pos((p).x, (p).y, z)

/**
 * @brief Performs a simple perspective projection of a 3D point onto a 2D
 * plane.
 *
 * This macro assumes a camera at the origin looking down the Z-axis, and
 * projects the point onto the Z=1 plane.
 *
 * @param p The `gm3Pos` to project.
 * @return A `gmPos` representing the 2D projected coordinates.
 */
#define gm3_pos_project_simple(p) gmpos((p).x / (p).z, (p).y / (p).z)

/**
 * @brief Calculates the midpoint between two `gm3Pos` points.
 * @param p A pointer to the `gm3Pos` to store the result.
 * @param va The first `gm3Pos` point.
 * @param vb The second `gm3Pos` point.
 */
static inline void gm3_pos_center(gm3Pos *p, const gm3Pos *va,
                                  const gm3Pos *vb) {
  p->x = (va->x + vb->x) / 2.0;
  p->y = (va->y + vb->y) / 2.0;
  p->z = (va->z + vb->z) / 2.0;
}
/**
 * @brief Calculates the centroid of three `gm3Pos` points (e.g., a triangle).
 * @param p A pointer to the `gm3Pos` to store the result.
 * @param va The first `gm3Pos` point.
 * @param vb The second `gm3Pos` point.
 * @param vc The third `gm3Pos` point.
 */
static inline void gm3_pos_center3(gm3Pos *p, const gm3Pos *va,
                                   const gm3Pos *vb, const gm3Pos *vc) {
  p->x = (va->x + vb->x + vc->x) / 3.0;
  p->y = (va->y + vb->y + vc->y) / 3.0;
  p->z = (va->z + vb->z + vc->z) / 3.0;
}

/**
 * @brief Subtracts one `gm3Pos` vector from another (va = va - vb).
 * @param va A pointer to the first `gm3Pos` vector (modified in place).
 * @param vb The second `gm3Pos` vector.
 */
static inline void gm3_pos_substract(gm3Pos *va, const gm3Pos *vb) {
  va->x -= vb->x;
  va->y -= vb->y;
  va->z -= vb->z;
}

/**
 * @brief Adds one `gm3Pos` vector to another (va = va + vb).
 * @param va A pointer to the first `gm3Pos` vector (modified in place).
 * @param vb The second `gm3Pos` vector.
 */
static inline void gm3_pos_add(gm3Pos *va, const gm3Pos *vb) {
  va->x += vb->x;
  va->y += vb->y;
  va->z += vb->z;
}

/**
 * @brief Normalizes a `gm3Pos` vector to a unit length (magnitude of 1).
 * @param v A pointer to the `gm3Pos` vector to normalize (modified in place).
 */
static inline void gm3_pos_normalize(gm3Pos *v) {
  double m = gm3_pos_magnitude(*v);
  if (m == 0)
    return; // Avoid division by zero
  v->x /= m;
  v->y /= m;
  v->z /= m;
}

/**
 * @brief Calculates the dot product of two `gm3Pos` vectors.
 * @param a The first `gm3Pos` vector.
 * @param b The second `gm3Pos` vector.
 * @return The dot product (scalar value).
 */
#define gm3_pos_dot(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)

/**
 * @brief Calculates the cross product of two `gm3Pos` vectors (a x b).
 * @param a The first `gm3Pos` vector.
 * @param b The second `gm3Pos` vector.
 * @return A new `gm3Pos` vector representing the cross product.
 */
#define gm3_pos_cross(a, b)                                                    \
  ((gm3Pos){(a).y * (b).z - (a).z * (b).y, (a).z * (b).x - (a).x * (b).z,      \
            (a).x * (b).y - (a).y * (b).x})

/**
 * @brief Resets the coordinates of a `gm3Pos` struct to (0, 0, 0).
 * @param p A pointer to the `gm3Pos` struct to reset.
 */
#define gm3_pos_reset(p) memset(p, 0, sizeof(*p))

/**
 * @brief Calculates the center of a given array of `gm3Pos` points.
 * @param arr An array of `gm3Pos` points.
 * @param n The number of points in the array.
 * @return A new `gm3Pos` representing the average center of the points.
 */
static inline gm3Pos gm3_pos_centerN(const gm3Pos *arr, const size_t n) {
  gm3Pos res = {0};
  if (n == 0)
    return res;
  double dn = (double)n;
  for (size_t i = 0; i < n; i++) {
    res.x += arr[i].x / dn;
    res.y += arr[i].y / dn;
    res.z += arr[i].z / dn;
  }
  return res;
}

/**
 * @brief Multiplies the components of a `gm3Pos` vector by the corresponding
 * components of another `gm3Pos` vector (component-wise multiplication).
 * @param res A pointer to the `gm3Pos` vector to modify (result stored here).
 * @param trans The `gm3Pos` vector to multiply by.
 */
void gm3_pos_mul(gm3Pos *res, const gm3Pos *trans) {
  res->x *= trans->x;
  res->y *= trans->y;
  res->z *= trans->z;
}
/**
 * @brief Multiplies the components of a `gm3Pos` vector by a scalar value.
 * @param res A pointer to the `gm3Pos` vector to modify (result stored here).
 * @param s The scalar value to multiply by.
 */
void gm3_pos_mul_scalar(gm3Pos *res, double s) {
  res->x *= s;
  res->y *= s;
  res->z *= s;
}

/**
 * @brief Converts a `gm3Pos` struct to a string representation for debugging.
 * @param str A pointer to a `gmStr` buffer to append the string to.
 * @param pos The `gm3Pos` struct to convert.
 * @return 0 on success.
 */
int gmg_pos3(gmStr *str, gm3Pos pos) {
  char buffer[128];
  snprintf(buffer, sizeof(buffer), "(gm3Pos){%.6g, %.6g, %.6g}", pos.x, pos.y,
           pos.z);
  gm_str_append(str, buffer);
  return 0;
}
#endif // GM_POSITION_H_INCLUDED
