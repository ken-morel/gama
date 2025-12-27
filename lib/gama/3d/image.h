#pragma once

#include "../color.h"
#include "../draw.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  gmPos *vertices;
  size_t n_vertices;

  gmColor *colors;
  size_t n_colors; // Should correspond to n_triangles

  size_t *triangles;
  size_t n_triangles;

  double *depths; // The average Z of each triangle
} gm3Image;

static inline gm3Image gm3_image() {
  return (gm3Image){.n_colors = 0,
                    .colors = NULL,
                    .n_vertices = 0,
                    .vertices = NULL,
                    .n_triangles = 0,
                    .triangles = NULL};
}

typedef struct {
  gmPos vertices[3];
  gmColor color;
  double depth;
} gm3TriangleImage;

static inline gm3Image gm3_image_init() { return (gm3Image){0}; }

void gm3_image_clear(gm3Image *i) {
  if (i->colors)
    free(i->colors);
  if (i->vertices)
    free(i->vertices);
  if (i->triangles)
    free(i->triangles);
  if (i->depths)
    free(i->depths);
  *i = (gm3Image){0};
}

typedef struct {
  size_t tri_idx;
  double z;
} _gmImageDepthEntry;

int _gm3_depth_compare(const void *a, const void *b) {
  _gmImageDepthEntry *ra = (_gmImageDepthEntry *)a;
  _gmImageDepthEntry *rb = (_gmImageDepthEntry *)b;
  // Sort Back-to-Front (highest Z first)
  if (rb->z < ra->z)
    return -1;
  if (rb->z > ra->z)
    return 1;
  return 0;
}

struct {
  short unsigned ignore_colors;
  gmColor ignored_colors[10];
} gm3DrawImage = {
    .ignore_colors = 0,
    .ignored_colors = {0},
};

int gm3_draw_image(gm3Image *m, double x, double y) {
  if (!m || m->n_triangles == 0)
    return 0;

  _gmImageDepthEntry *sort_buffer =
      malloc(sizeof(_gmImageDepthEntry) * m->n_triangles);
  if (!sort_buffer)
    return -1;

  for (size_t i = 0; i < m->n_triangles; i++) {
    sort_buffer[i].tri_idx = i;
    sort_buffer[i].z = m->depths[i];
  }

  qsort(sort_buffer, m->n_triangles, sizeof(_gmImageDepthEntry),
        _gm3_depth_compare);

  for (size_t i = 0; i < m->n_triangles; i++) {

    size_t tidx = sort_buffer[i].tri_idx;

    // Correctly reference the vertices using the triangle index buffer
    gmPos v1 = m->vertices[m->triangles[tidx * 3 + 0]];
    gmPos v2 = m->vertices[m->triangles[tidx * 3 + 1]];
    gmPos v3 = m->vertices[m->triangles[tidx * 3 + 2]];
    for (size_t c = 0; c < gm3DrawImage.ignore_colors; c++)
      if (gm3DrawImage.ignored_colors[c] == m->colors[tidx])
        continue;

    gm_draw_triangle(v1.x + x, v1.y + y, v2.x + x, v2.y + y, v3.x + x, v3.y + y,
                     m->colors[tidx]);
  }

  free(sort_buffer);
  return 1;
}
