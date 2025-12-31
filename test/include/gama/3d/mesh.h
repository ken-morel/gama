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

gmStr gmg_face(gm3MeshFace f) {
  gmStr str = gm_str();
  char buffer[256];

  gm_str_append(&str, "(gm3MeshFace){");

  // Vertices
  sprintf(buffer, ".vertices = {%zu, %zu, %zu}, ", f.vertices[0], f.vertices[1],
          f.vertices[2]);
  gm_str_append(&str, buffer);

  // Material Indices
  sprintf(buffer, ".material = %d, .material_file = %d, ", f.material,
          f.material_file);
  gm_str_append(&str, buffer);

  // Normal (uses gmg_pos3)
  gm_str_append(&str, ".normal = ");
  gmStr s_norm = gmg_pos3(f.normal);
  gm_str_append(&str, s_norm.content);
  // (free(s_norm.content) here if your gmStr allocates memory)

  gm_str_append(&str, "}");
  return str;
}

gmStr gmg_mesh(gm3Mesh m) {
  gmStr str = gm_str();
  char buffer[1024];

  // 1. Open Struct R-Value
  gm_str_append(&str, "(gm3Mesh){\n");

  // 2. Vertices
  sprintf(buffer, "  .n_vertices = %zu,\n", m.n_vertices);
  gm_str_append(&str, buffer);

  if (m.n_vertices > 0) {
    gm_str_append(&str, "  .vertices = (gm3Pos[]){\n");
    for (size_t i = 0; i < m.n_vertices; i++) {
      gmStr s_pos = gmg_pos3(m.vertices[i]);

      gm_str_append(&str, "    ");
      gm_str_append(&str, s_pos.content); // Append generated vector string
      gm_str_clear(&s_pos);

      if (i < m.n_vertices - 1)
        gm_str_append(&str, ",\n");
      else
        gm_str_append(&str, "\n");
    }
    gm_str_append(&str, "  },\n");
  } else {
    gm_str_append(&str, "  .vertices = NULL,\n");
  }

  // 3. Faces
  sprintf(buffer, "  .n_faces = %zu,\n", m.n_faces);
  gm_str_append(&str, buffer);

  if (m.n_faces > 0) {
    gm_str_append(&str, "  .faces = (gm3MeshFace[]){\n");
    for (size_t i = 0; i < m.n_faces; i++) {
      gmStr s_face = gmg_face(m.faces[i]);

      gm_str_append(&str, "    ");
      gm_str_append(&str, s_face.content); // Append generated face string
      gm_str_clear(&s_face);

      if (i < m.n_faces - 1)
        gm_str_append(&str, ",\n");
      else
        gm_str_append(&str, "\n");
    }
    gm_str_append(&str, "  },\n");
  } else {
    gm_str_append(&str, "  .faces = NULL,\n");
  }

  // 4. Normals
  sprintf(buffer, "  .n_normals = %zu,\n", m.n_normals);
  gm_str_append(&str, buffer);

  if (m.n_normals > 0) {
    gm_str_append(&str, "  .normals = (gm3Pos[]){\n");
    for (size_t i = 0; i < m.n_normals; i++) {
      gmStr s_norm = gmg_pos3(m.normals[i]);

      gm_str_append(&str, "    ");
      gm_str_append(&str, s_norm.content);
      gm_str_clear(&s_norm);

      if (i < m.n_normals - 1)
        gm_str_append(&str, ",\n");
      else
        gm_str_append(&str, "\n");
    }
    gm_str_append(&str, "  },\n");
  } else {
    gm_str_append(&str, "  .normals = NULL,\n");
  }

  // 5. Material Libraries (mtllibs)
  // This recursively generates the libraries contained in the mesh
  sprintf(buffer, "  .n_mtllibs = %zu,\n", m.n_mtllibs);
  gm_str_append(&str, buffer);

  if (m.n_mtllibs > 0) {
    gm_str_append(&str, "  .mtllibs = (gm3MtlFile[]){\n");
    for (size_t i = 0; i < m.n_mtllibs; i++) {
      // Assuming gmg_mtllib exists and generates (gm3MtlLib){...}
      // Note: check if your struct is named gm3MtlFile or gm3MtlLib
      gmStr s_lib = gmg_mtllib(m.mtllibs[i]);

      gm_str_append(&str, "    ");
      gm_str_append(&str, s_lib.content);
      gm_str_clear(&s_lib);

      if (i < m.n_mtllibs - 1)
        gm_str_append(&str, ",\n");
      else
        gm_str_append(&str, "\n");
    }
    gm_str_append(&str, "  }\n");
  } else {
    gm_str_append(&str, "  .mtllibs = NULL\n");
  }

  // 6. Close Struct
  gm_str_append(&str, "}");
  return str;
}
