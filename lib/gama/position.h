#ifndef GM_POSITION_H_INCLUDED
#define GM_POSITION_H_INCLUDED

#include "_math.h"

/**
 * @brief Represents a 2D position or vector.
 */
typedef struct {
  double x, y; /**< The X and Y coordinates. */
} gmPos;

/**
 * @brief Creates a new `gmPos` struct with the given coordinates.
 * @param x The X-coordinate.
 * @param y The Y-coordinate.
 * @return A new `gmPos` instance.
 */
static inline gmPos gmpos(double x, double y) {
  gmPos p = {x, y};
  return p;
}

/**
 * @brief Sets the coordinates of an existing `gmPos` struct.
 * @param p A pointer to the `gmPos` struct to modify.
 * @param x The new X-coordinate.
 * @param y The new Y-coordinate.
 */
static inline void gm_pos_set(gmPos *p, double x, double y) {
  p->x = x;
  p->y = y;
}

/**
 * @brief Resets the coordinates of a `gmPos` struct to (0, 0).
 * @param p A pointer to the `gmPos` struct to reset.
 */
static inline void gm_pos_reset(gmPos *p) {
  p->x = 0;
  p->y = 0;
}

/**
 * @brief Calculates the magnitude (length) of a `gmPos` vector.
 * @param p The `gmPos` vector.
 * @return The magnitude of the vector.
 */
static inline double gm_pos_magniture(gmPos p) {
  return sqrt(p.x * p.x + p.y * p.y);
}

/**
 * @brief Calculates the Euclidean distance between two `gmPos` points.
 * @param a The first `gmPos` point.
 * @param b The second `gmPos` point.
 * @return The distance between the two points.
 */
static inline double gm_pos_distance(gmPos a, gmPos b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/**
 * Calculate the area of a triangle formed by 3 2D points using the cross
 * product formula
 *
 * @param a First vertex of the triangle
 * @param b Second vertex of the triangle
 * @param c Third vertex of the triangle
 * @return The area of the triangle (always positive)
 */
double gm_triangle_area(gmPos a, gmPos b, gmPos c) {
  double ab_x = b.x - a.x;
  double ab_y = b.y - a.y;
  double ac_x = c.x - a.x;
  double ac_y = c.y - a.y;
  return 0.5 * fabs(ab_x * ac_y - ab_y * ac_x);
}

#endif // GM_POSITION_H_INCLUDED
