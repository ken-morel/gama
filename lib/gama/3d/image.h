#pragma once

#include "../color.h"
#include "../draw.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  gmPos *vertices; // projected 2D coordinates
  size_t n_vertices;

  gmColor *colors;
  size_t n_colors;

  size_t *triangles;
  size_t n_triangles;

  double *depths; // the z of each coordinate
} gm3Image;

typedef struct {
  gmPos vertices[3];
  gmColor color;
  double depth;
} gm3TriangleImage;

// Helper structure for sorting
typedef struct {
  size_t tri_idx;
  double z;
} _gmImageDepthEntry;

// Comparator for qsort: Sorts descending (furthest Z first)
int _gm3_depth_compare(const void *a, const void *b) {
  _gmImageDepthEntry *ra = (_gmImageDepthEntry *)a;
  _gmImageDepthEntry *rb = (_gmImageDepthEntry *)b;
  if (rb->z < ra->z)
    return -1;
  if (rb->z > ra->z)
    return 1;
  return 0;
}

int gm3_draw_image(gm3Image *m, double x, double y) {
  if (!m || m->n_triangles == 0)
    return 0;

  // 1. Create a sorting buffer
  _gmImageDepthEntry *sort_buffer =
      malloc(sizeof(_gmImageDepthEntry) * m->n_triangles);
  if (!sort_buffer)
    return -1;

  // 2. Calculate average Z for each triangle
  for (size_t i = 0; i < m->n_triangles; i++) {
    sort_buffer[i].tri_idx = i;
    sort_buffer[i].z = m->depths[i];
  }

  // 3. Sort triangles by depth (Back-to-Front)
  qsort(sort_buffer, m->n_triangles, sizeof(_gmImageDepthEntry),
        _gm3_depth_compare);

  // 4. Draw triangles in sorted order
  for (size_t i = 0; i < m->n_triangles; i++) {
    size_t actual_idx = sort_buffer[i].tri_idx;

    // Get the three vertices for this triangle
    gmPos v1 = m->vertices[m->triangles[actual_idx * 3 + 0]];
    gmPos v2 = m->vertices[m->triangles[actual_idx * 3 + 1]];
    gmPos v3 = m->vertices[m->triangles[actual_idx * 3 + 2]];

    // Get the pre-calculated color
    gmColor c = m->colors[actual_idx];

    // Call the 2D engine drawing function with the provided (x, y) offset
    gm_draw_triangle(v1.x + x, v1.y + y, v2.x + x, v2.y + y, v3.x + x, v3.y + y,
                     c);
  }

  free(sort_buffer);
  return 1;
}
