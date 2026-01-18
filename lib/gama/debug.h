/**
 * @file debug.h
 * @brief Provides macros and functions for debugging output in Gama.
 *
 * This file offers utilities to print the state of various Gama data structures
 * to the console, aiding in debugging and development.
 */
#ifndef GM_DEBUG_H_INCLUDED
#define GM_DEBUG_H_INCLUDED
#include "3d/position.h"
#include "color.h"
#include "position.h" // For gmPos
#include <stdio.h>

/**
 * @def gmdi(fn, obj)
 * @brief Macro to print debug information for an object without a newline.
 * @param fn The debug formatting function (e.g., `color`, `pos2`, `pos3`, `mesh`).
 * @param obj The object to print.
 */
#define gmdi(fn, obj) gmd(fn, obj, "")
/**
 * @def gmdn(fn, obj)
 * @brief Macro to print debug information for an object with a newline.
 * @param fn The debug formatting function (e.g., `color`, `pos2`, `pos3`, `mesh`).
 * @param obj The object to print.
 */
#define gmdn(fn, obj) gmd(fn, obj, "\n")
/**
 * @def gmd(fn, obj, e)
 * @brief Generic macro to print debug information for an object.
 *
 * This macro calls a specific `gmd_` formatting function, prints the result
 * to stdout, and appends an optional end string.
 *
 * @param fn The debug formatting function to use (e.g., `color`, `pos2`, `pos3`, `mesh`).
 * @param obj The object to print debug information for.
 * @param e An optional string to append after the debug output (e.g., "\n").
 */
#define gmd(fn, obj, e)                                                        \
  do {                                                                         \
    char __gmd_str[200];                                                       \
    /* Use sizeof the buffer, not sizeof the number 200 */                     \
    gmd_##fn(__gmd_str, sizeof(__gmd_str), obj);                               \
    printf("<%s>%s", __gmd_str, e);                                            \
  } while (0)

/**
 * @brief Formats a `gmColor` into a human-readable string for debugging.
 * @param s The buffer to write the formatted string into.
 * @param n The size of the buffer.
 * @param c The `gmColor` to format.
 * @return The number of characters written to the buffer.
 */
int gmd_color(char *s, size_t n, gmColor c) {
  return snprintf(s, n, "gmPos(0x%#08X|r: %d, g: %d, b: %d, a: %d)", c,
                  gm_red(c), gm_green(c), gm_blue(c), gm_alpha(c));
}

/**
 * @brief Formats a `gm3Pos` (3D position) into a human-readable string for debugging.
 * @param s The buffer to write the formatted string into.
 * @param n The size of the buffer.
 * @param p The `gm3Pos` to format.
 * @return The number of characters written to the buffer.
 */
int gmd_pos3(char *s, size_t n, gm3Pos p) {
  return snprintf(s, n, "gm3Pos(x: %.lf, y: %.lf, z: %.lf)", p.x, p.y, p.z);
}
/**
 * @brief Formats a `gmPos` (2D position) into a human-readable string for debugging.
 * @param s The buffer to write the formatted string into.
 * @param n The size of the buffer.
 * @param p The `gmPos` to format.
 * @return The number of characters written to the buffer.
 */
int gmd_pos2(char *s, size_t n, gmPos p) {
  return snprintf(s, n, "gmPos(x: %.lf, y: %.lf)", p.x, p.y);
}

#include "3d/mesh.h" // For gm3Mesh

/**
 * @brief Formats a `gm3Mesh` into a human-readable string for debugging.
 * @param s The buffer to write the formatted string into.
 * @param n The size of the buffer.
 * @param m The `gm3Mesh` to format.
 * @return The number of characters written to the buffer.
 */
int gmd_mesh(char *s, size_t n, gm3Mesh m) {
  return snprintf(
      s, n,
      "gm3Mesh(n_vertices: %zu, n_faces: %zu, n_normals: %zu, n_mtllibs: "
      "%zu, ...)",
      m.n_vertices, m.n_faces, m.n_normals, m.n_mtllibs);
}
#endif // GM_DEBUG_H_INCLUDED
