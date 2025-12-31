#pragma once

#include "../position.h"
#include "../str.h"
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

    min.z = fmin(p.z, min.z);
    max.z = fmax(p.z, max.z);
  }

  gm3Pos center;
  gm3_pos_center(&center, &max, &min);

  for (size_t v = 0; v < m->n_vertices; v++)
    gm3_pos_substract(&m->vertices[v], &center);
  return 0;
}
gmStr gmg_mesh(gm3Mesh m) {
  gmStr str = gm_str();
  char buffer[1024] = {0};

  sprintf(buffer, "const gm3Mesh triangle = {\n");
  gm_str_append(&str, buffer);

  sprintf(buffer, "  .n_vertices = %zu,\n", m.n_vertices);
  gm_str_append(&str, buffer);

  sprintf(buffer, "  .vertices = (gm3Pos[]){\n");
  gm_str_append(&str, buffer);
  for (size_t i = 0; i < m.n_vertices; i++) {
    sprintf(buffer, "    {%lf, %lf, %lf}", m.vertices[i].x, m.vertices[i].y,
            m.vertices[i].z);
    gm_str_append(&str, buffer);
    if (i < m.n_vertices - 1)
      gm_str_append(&str, ",\n");
    else
      gm_str_append(&str, "\n");
  }
  sprintf(buffer, "  },\n");
  gm_str_append(&str, buffer);

  // 3. Faces
  sprintf(buffer, "  .n_faces = %zu,\n", m.n_faces);
  gm_str_append(&str, buffer);

  sprintf(buffer, "  .faces = (gm3MeshFace[]){\n");
  gm_str_append(&str, buffer);

  for (size_t i = 0; i < m.n_faces; i++) {
    sprintf(buffer,
            "    {.vertices = {%zu, %zu, %zu}, "
            ".material = %d, .material_file = %d, "
            ".normal = {%lf, %lf, %lf}}",
            m.faces[i].vertices[0], m.faces[i].vertices[1],
            m.faces[i].vertices[2], m.faces[i].material,
            m.faces[i].material_file, m.faces[i].normal.x, m.faces[i].normal.y,
            m.faces[i].normal.z);

    gm_str_append(&str, buffer);

    if (i < m.n_faces - 1)
      gm_str_append(&str, ",\n");
    else
      gm_str_append(&str, "\n");
  }
  sprintf(buffer, "  },\n");
  gm_str_append(&str, buffer);

  sprintf(buffer, "  .n_normals = %zu,\n", m.n_normals);
  gm_str_append(&str, buffer);

  if (m.n_normals > 0) {
    sprintf(buffer, "  .normals = (gm3Pos[]){\n");
    gm_str_append(&str, buffer);
    for (size_t i = 0; i < m.n_normals; i++) {
      sprintf(buffer, "    {%lf, %lf, %lf}", m.normals[i].x, m.normals[i].y,
              m.normals[i].z);
      gm_str_append(&str, buffer);
      if (i < m.n_normals - 1)
        gm_str_append(&str, ",\n");
      else
        gm_str_append(&str, "\n");
    }
    sprintf(buffer, "  },\n");
    gm_str_append(&str, buffer);
  } else {
    sprintf(buffer, "  .normals = NULL,\n");
    gm_str_append(&str, buffer);
  }

  sprintf(buffer, "  .n_mtl_files = 0, .mtl_files = NULL\n");
  gm_str_append(&str, buffer);

  sprintf(buffer, "};\n");
  gm_str_append(&str, buffer);

  return str;
}
