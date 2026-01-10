#pragma once

#include "../position.h"
#include "../str.h"
#include "mtl.h"
#include "position.h"
#include <float.h>

typedef struct {
  size_t vertices[3];
  long uvs[3];
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

int32_t gmg_face(gmStr *str, gm3MeshFace f) {
  char buffer[256];

  gm_str_append(str, "(gm3MeshFace){");

  // Vertices
  snprintf(buffer, sizeof(buffer), ".vertices = {%zu, %zu, %zu}, ",
           f.vertices[0], f.vertices[1], f.vertices[2]);
  gm_str_append(str, buffer);

  // Material Indices
  snprintf(buffer, sizeof(buffer), ".material = %d, .material_file = %d, ",
           f.material, f.material_file);
  gm_str_append(str, buffer);

  // Normal (uses gmg_pos3)
  gm_str_append(str, ".normal = ");
  gmg_pos3(str, f.normal);

  gm_str_append(str, "}");
  return 0;
}

int32_t gmg_mesh(gmStr *str, gm3Mesh m) {
  char buffer[1024];

  // 1. Open Struct R-Value
  gm_str_append(str, "(gm3Mesh){\n");

  // 2. Vertices
  snprintf(buffer, sizeof(buffer), "  .n_vertices = %zu,\n", m.n_vertices);
  gm_str_append(str, buffer);

  if (m.n_vertices > 0) {
    gm_str_append(str, "  .vertices = (gm3Pos[]){\n");
    for (size_t i = 0; i < m.n_vertices; i++) {

      gm_str_append(str, "    ");
      gmg_pos3(str, m.vertices[i]);

      if (i < m.n_vertices - 1)
        gm_str_append(str, ",\n");
      else
        gm_str_append(str, "\n");
    }
    gm_str_append(str, "  },\n");
  } else {
    gm_str_append(str, "  .vertices = NULL,\n");
  }

  // 3. Faces
  snprintf(buffer, sizeof(buffer), "  .n_faces = %zu,\n", m.n_faces);
  gm_str_append(str, buffer);

  if (m.n_faces > 0) {
    gm_str_append(str, "  .faces = (gm3MeshFace[]){\n");
    for (size_t i = 0; i < m.n_faces; i++) {

      gm_str_append(str, "    ");
      gmg_face(str, m.faces[i]);

      if (i < m.n_faces - 1)
        gm_str_append(str, ",\n");
      else
        gm_str_append(str, "\n");
    }
    gm_str_append(str, "  },\n");
  } else {
    gm_str_append(str, "  .faces = NULL,\n");
  }

  // 4. Normals
  snprintf(buffer, sizeof(buffer), "  .n_normals = %zu,\n", m.n_normals);
  gm_str_append(str, buffer);

  if (m.n_normals > 0) {
    gm_str_append(str, "  .normals = (gm3Pos[]){\n");
    for (size_t i = 0; i < m.n_normals; i++) {

      gm_str_append(str, "    ");
      gmg_pos3(str, m.normals[i]);

      if (i < m.n_normals - 1)
        gm_str_append(str, ",\n");
      else
        gm_str_append(str, "\n");
    }
    gm_str_append(str, "  },\n");
  } else {
    gm_str_append(str, "  .normals = NULL,\n");
  }

  // 5. Material Libraries (mtllibs)
  // This recursively generates the libraries contained in the mesh
  snprintf(buffer, sizeof(buffer), "  .n_mtllibs = %zu,\n", m.n_mtllibs);
  gm_str_append(str, buffer);

  if (m.n_mtllibs > 0) {
    gm_str_append(str, "  .mtllibs = (gm3MtlLib[]){\n");
    for (size_t i = 0; i < m.n_mtllibs; i++) {
      // Assuming gmg_mtllib exists and generates (gm3MtlLib){...}

      gm_str_append(str, "    ");
      gmg_mtllib(str, m.mtllibs[i]);

      if (i < m.n_mtllibs - 1)
        gm_str_append(str, ",\n");
      else
        gm_str_append(str, "\n");
    }
    gm_str_append(str, "  }\n");
  } else {
    gm_str_append(str, "  .mtllibs = NULL\n");
  }

  // 6. Close Struct
  gm_str_append(str, "}");
  return 0;
}
