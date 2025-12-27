#pragma once

#include "../color.h"
#include "../draw.h"
#include "position.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  gm3Pos *vertices; // projected 2D coordinates (with z for depth sorting)
  size_t n_vertices;

  gmColor *colors;
  size_t n_colors;

  size_t *triangles;
  size_t n_triangles;
} gm3Image;

typedef struct {
  gm3Pos vertices[3];
  gmColor color;
  int visible;
} gm3TriangleImage;

// Helper structure for sorting
typedef struct {
  size_t tri_idx;
  double avg_z;
} _gm3DepthEntry;

// Comparator for qsort: Sorts descending (furthest Z first)
int _gm3_depth_compare(const void *a, const void *b) {
  _gm3DepthEntry *ra = (_gm3DepthEntry *)a;
  _gm3DepthEntry *rb = (_gm3DepthEntry *)b;
  if (rb->avg_z < ra->avg_z)
    return -1;
  if (rb->avg_z > ra->avg_z)
    return 1;
  return 0;
}

int gm3_draw_image(gm3Image *m, double x, double y) {
  if (!m || m->n_triangles == 0)
    return 0;

  // 1. Create a sorting buffer
  _gm3DepthEntry *sort_buffer = malloc(sizeof(_gm3DepthEntry) * m->n_triangles);
  if (!sort_buffer)
    return -1;

  // 2. Calculate average Z for each triangle
  for (size_t i = 0; i < m->n_triangles; i++) {
    size_t v0_idx = m->triangles[i * 3 + 0];
    size_t v1_idx = m->triangles[i * 3 + 1];
    size_t v2_idx = m->triangles[i * 3 + 2];

    double z0 = m->vertices[v0_idx].z;
    double z1 = m->vertices[v1_idx].z;
    double z2 = m->vertices[v2_idx].z;

    sort_buffer[i].tri_idx = i;
    sort_buffer[i].avg_z = (z0 + z1 + z2) / 3.0;
  }

  // 3. Sort triangles by depth (Back-to-Front)
  qsort(sort_buffer, m->n_triangles, sizeof(_gm3DepthEntry),
        _gm3_depth_compare);

  // 4. Draw triangles in sorted order
  for (size_t i = 0; i < m->n_triangles; i++) {
    size_t actual_idx = sort_buffer[i].tri_idx;

    // Get the three vertices for this triangle
    gm3Pos v1 = m->vertices[m->triangles[actual_idx * 3 + 0]];
    gm3Pos v2 = m->vertices[m->triangles[actual_idx * 3 + 1]];
    gm3Pos v3 = m->vertices[m->triangles[actual_idx * 3 + 2]];

    // Get the pre-calculated color
    gmColor c = m->colors[actual_idx];

    // Call the 2D engine drawing function with the provided (x, y) offset
    gm_draw_triangle(v1.x + x, v1.y + y, v2.x + x, v2.y + y, v3.x + x, v3.y + y,
                     c);
  }

  free(sort_buffer);
  return 1;
}
