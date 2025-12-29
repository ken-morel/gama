#pragma once

#include "mtl.h"
#include "position.h"
#include <float.h>

typedef struct {
  size_t vertices[3];
  long texs[3];
  int material;
  int material_file;
  gm3Pos normal;
} gm3MeshFace;

typedef struct {
  gm3Pos *vertices;
  size_t n_vertices;

  gm3MeshFace *faces;
  size_t n_faces;

  gm3Pos *normals;
  size_t n_normals;

  gmPos *texs;
  size_t n_texs;

  gm3MtlLib *mtllibs;
  size_t n_mtllibs;
} gm3Mesh;
void gm3_mesh_free(gm3Mesh *m) {
  if (m->vertices)
    free(m->vertices);
  if (m->faces)
    free(m->faces);
  if (m->normals)
    free(m->normals);
  if (m->texs)
    free(m->texs);
  if (m->mtllibs)
    free(m->mtllibs);
  memset(m, 0, sizeof(gm3Mesh));
}

int gm3_mesh_center(gm3Mesh *m) {
  if (!m)
    return -1;
  gm3Pos max = {DBL_MIN, DBL_MIN, DBL_MIN};
  gm3Pos min = {DBL_MAX, DBL_MAX, DBL_MAX};

  for (size_t v = 0; v < m->n_vertices; v++) {
    gm3Pos p = m->vertices[v];
    min.x = fmin(p.x, min.x);
    max.x = fmax(p.x, max.x);

    min.y = fmin(p.y, min.y);
    max.y = fmax(p.y, max.y);

    min.x = fmin(p.x, min.x);
    max.x = fmax(p.x, max.x);
  }

  gm3Pos center;
  gm3_pos_center(&center, &max, &min);

  for (size_t v = 0; v < m->n_vertices; v++)
    gm3_pos_substract(&m->vertices[v], &center);
  return 0;
}
