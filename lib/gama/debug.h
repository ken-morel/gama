#pragma once

#include "3d/mesh.h"
#include "3d/position.h"
#include "color.h"
#include <stdio.h>

#define gmdi(fn, obj) gmd(fn, obj, "")
#define gmdn(fn, obj) gmd(fn, obj, "\n")
#define gmd(fn, obj, e)                                                        \
  do {                                                                         \
    char __gmd_str[200] = {0};                                                 \
    /* Use sizeof the buffer, not sizeof the number 200 */                     \
    gmd_##fn(__gmd_str, sizeof(__gmd_str), obj);                               \
    printf("<%s>%s", __gmd_str, e);                                            \
  } while (0) // Removed trailing semicolon to require one in code

int gmd_color(char *s, size_t n, gmColor c) {
  return snprintf(s, n, "gmPos(0x%#08X|r: %d, g: %d, b: %d, a: %d)", c,
                  gm_red(c), gm_green(c), gm_blue(c), gm_alpha(c));
}

int gmd_pos3(char *s, size_t n, gm3Pos p) {
  return snprintf(s, n, "gm3Pos(x: %.lf, y: %.lf, z: %.lf)", p.x, p.y, p.z);
}
int gmd_pos2(char *s, size_t n, gmPos p) {
  return snprintf(s, n, "gmPos(x: %.lf, y: %.lf)", p.x, p.y);
}

int gmd_mesh(char *s, size_t n, gm3Mesh m) {
  return snprintf(
      s, n,
      "gm3Mesh(n_vertices: %zu, n_faces: %zu, n_normals: %zu, n_mtllibs: "
      "%zu, ...)",
      m.n_vertices, m.n_faces, m.n_normals, m.n_mtllibs);
}
