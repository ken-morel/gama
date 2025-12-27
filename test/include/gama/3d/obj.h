#pragma once

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"

// --- Structures ---

typedef struct {
  size_t total_v;
  size_t total_vn;
  size_t total_vt;
} gm3ObjContext;

typedef struct {
  char path[256];
  gm3Mesh *objects;
  size_t n_objects;
} gm3ObjFile;

typedef struct {
  char type;
  double points[3];
  long indices[64][3]; // [v, vt, vn]
  size_t n_indices;
} gm3ObjLine;

static inline void _gm3_vec_sub(gm3Pos *r, gm3Pos a, gm3Pos b) {
  r->x = a.x - b.x;
  r->y = a.y - b.y;
  r->z = a.z - b.z;
}

static inline void _gm3_vec_normalize(gm3Pos *v) {
  double len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  if (len > 1e-9) {
    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}

static inline char *skip_ws(char *s) {
  while (*s && isspace((unsigned char)*s))
    s++;
  return s;
}

static long parse_idx(char **ptr, size_t total) {
  if (!*ptr || isspace((unsigned char)**ptr))
    return 0;
  char *end;
  long val = strtol(*ptr, &end, 10);
  *ptr = end;
  if (val < 0)
    return (long)total + val + 1;
  return val;
}

// --- Parsing Logic ---

int gm3_obj_read_line(FILE *f, gm3ObjLine *ln, gm3ObjContext *ctx) {
  char buf[1024];
  ln->type = '?';
  ln->n_indices = 0;

  if (!fgets(buf, sizeof(buf), f))
    return -2; // EOF

  char *p = skip_ws(buf);
  if (*p == '\0' || *p == '#')
    return 0;

  if (p[0] == 'v' && isspace((unsigned char)p[1])) {
    ln->type = 'v';
    p = skip_ws(p + 1);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = strtod(p, &p);
  } else if (p[0] == 'v' && p[1] == 'n') {
    ln->type = 'n';
    p = skip_ws(p + 2);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = strtod(p, &p);
  } else if (p[0] == 'f' && isspace((unsigned char)p[1])) {
    ln->type = 'f';
    p = skip_ws(p + 1);
    while (*p != '\0' && *p != '\n' && ln->n_indices < 64) {
      ln->indices[ln->n_indices][0] = parse_idx(&p, ctx->total_v);
      if (*p == '/') {
        p++;
        if (*p != '/')
          ln->indices[ln->n_indices][1] = parse_idx(&p, ctx->total_vt);
        if (*p == '/') {
          p++;
          ln->indices[ln->n_indices][2] = parse_idx(&p, ctx->total_vn);
        }
      }
      ln->n_indices++;
      p = skip_ws(p);
    }
  } else if (p[0] == 'o' || p[0] == 'g') {
    ln->type = 'o';
  }
  return 0;
}

// --- Memory Management ---

void gm3_free_mesh(gm3Mesh *m) {
  if (!m)
    return;
  if (m->vertices)
    free(m->vertices);
  if (m->faces)
    free(m->faces);
  if (m->normals)
    free(m->normals);
  memset(m, 0, sizeof(gm3Mesh));
}

void gm3_free_obj(gm3ObjFile *obj) {
  if (!obj)
    return;
  if (obj->objects) {
    for (size_t i = 0; i < obj->n_objects; i++) {
      gm3_free_mesh(&obj->objects[i]);
    }
    free(obj->objects);
  }
  obj->n_objects = 0;
}

// we make the cordinates of the vertices of the mesh
// to be about the mesh's center
int gm3_center_mesh_vertices(gm3Mesh *m) {
  gm3Pos center = {0, 0, 0};
  size_t i;
  double ratio = 1.0 / (double)m->n_vertices;
  for (i = 0; i < m->n_vertices; i++) {
    center.x += m->vertices[i].x * ratio;
    center.y += m->vertices[i].y * ratio;
    center.z += m->vertices[i].z * ratio;
  }
  // and that's it, we have the center, we can now
  // substract it in each vertex
  for (i = 0; i < m->n_vertices; i++) {
    m->vertices[i] = gm3_pos_minus(m->vertices[i], center);
  }
  return 0;
}
// --- High Level Loading ---
//

int gm3_obj_load_mesh(gm3Mesh *m, FILE *f, gm3ObjContext *ctx) {
  long mesh_start_pos = ftell(f);
  gm3ObjLine ln;
  size_t local_v = 0, local_vn = 0, local_f = 0;

  // Pass 1: Scan for counts
  while (gm3_obj_read_line(f, &ln, ctx) == 0) {
    if (ln.type == 'o' && local_v > 0)
      break;
    if (ln.type == 'v') {
      local_v++;
      ctx->total_v++;
    }
    if (ln.type == 'n') {
      local_vn++;
      ctx->total_vn++;
    }
    if (ln.type == 'f')
      local_f += (ln.n_indices >= 3 ? ln.n_indices - 2 : 0);
  }

  if (local_v == 0)
    return 0; // Empty object

  m->n_vertices = local_v;
  m->n_faces = local_f;
  m->vertices = calloc(local_v, sizeof(gm3Pos));
  m->faces = calloc(local_f, sizeof(gm3MeshFace));
  m->normals = calloc(local_f, sizeof(gm3Pos));

  if (!m->vertices || !m->faces || !m->normals) {
    gm3_free_mesh(m);
    return -3; // Memory error
  }

  // Pass 2: Data Load
  fseek(f, mesh_start_pos, SEEK_SET);
  size_t cv = 0, cf = 0, cvn = 0;
  gm3Pos *temp_vn = calloc(local_vn + 1, sizeof(gm3Pos));
  size_t v_offset = ctx->total_v - local_v;
  size_t vn_offset = ctx->total_vn - local_vn;

  while (gm3_obj_read_line(f, &ln, ctx) == 0) {
    if (ln.type == 'o' && cv > 0)
      break;
    if (ln.type == 'v')
      m->vertices[cv++] = (gm3Pos){ln.points[0], ln.points[1], ln.points[2]};
    if (ln.type == 'n' && temp_vn)
      temp_vn[cvn++] = (gm3Pos){ln.points[0], ln.points[1], ln.points[2]};
    if (ln.type == 'f') {
      // we will devide the polygon into triangles
      for (size_t i = 0; i < ln.n_indices - 2; i++) {
        gm3MeshFace *face = &m->faces[cf]; // a new face
        face->vertices[0] =
            ln.indices[0][0] - v_offset - 1; // this remains constant
        face->vertices[1] =
            ln.indices[i + 1][0] - v_offset - 1; // attach to the previous face
        face->vertices[2] = ln.indices[i + 2][0] - v_offset - 1;

        gm3Pos n = {0, 0, 0};
        // If the OBJ provides normals, use them
        // ignore the provided normal: n = temp_vn[ln.indices[0][2] - vn_offset
        // - 1]; Fallback to cross product calculation
        gm3Pos e1, e2;
        // find the face normal by computing (a - b) x (a - c)
        _gm3_vec_sub(&e1, m->vertices[face->vertices[1]],
                     m->vertices[face->vertices[0]]);
        _gm3_vec_sub(&e2, m->vertices[face->vertices[2]],
                     m->vertices[face->vertices[0]]);
        n = gm3_pos_cross(e1, e2);
        _gm3_vec_normalize(&n);

        // Normal Deduplication Logic (Basic)
        size_t found_n = cf;
        for (size_t j = 0; j < cf; j++) {
          if (fabs(m->normals[j].x - n.x) < 0.001 &&
              fabs(m->normals[j].y - n.y) < 0.001 &&
              fabs(m->normals[j].z - n.z) < 0.001) {
            found_n = j;
            break;
          }
        }
        m->normals[found_n] = n;
        face->normal = found_n;
        cf++;
      }
    }
  }
  if (temp_vn)
    free(temp_vn);
  return 0;
}

int gm3_center_obj_vertices(gm3ObjFile *obj) {
  if (!obj)
    return -1;
  // firstly find the center
  gm3Pos center = {0, 0, 0};
  long double n_total_vertices = 0; // find the total number of vertices
  for (size_t i = 0; i < obj->n_objects; i++) {
    n_total_vertices += obj->objects[i].n_vertices;
  }
  for (size_t obj_i = 0; obj_i < obj->n_objects; obj_i++)
    for (size_t vertex = 0; vertex < obj->objects[obj_i].n_vertices; vertex++) {
      center.x += obj->objects[obj_i].vertices[vertex].x / n_total_vertices;
      center.y += obj->objects[obj_i].vertices[vertex].y / n_total_vertices;
      center.z += obj->objects[obj_i].vertices[vertex].z / n_total_vertices;
    }
  // now we have the center, we center all the vertices
  for (size_t obj_i = 0; obj_i < obj->n_objects; obj_i++)
    for (size_t vertex = 0; vertex < obj->objects[obj_i].n_vertices; vertex++) {
      obj->objects[obj_i].vertices[vertex].x -= center.x;
      obj->objects[obj_i].vertices[vertex].y -= center.y;
      obj->objects[obj_i].vertices[vertex].z -= center.z;
    }
  return 0;
}

int gm3_load_obj(gm3ObjFile *obj, const char *path) {
  FILE *f = fopen(path, "r");
  if (!f)
    return -1;

  memset(obj, 0, sizeof(gm3ObjFile));
  snprintf(obj->path, sizeof(obj->path), "%s", path);

  // Count objects
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (line[0] == 'o' && isspace((unsigned char)line[1]))
      obj->n_objects++;
  }
  if (obj->n_objects == 0)
    obj->n_objects = 1; // Default to 1 if 'o' is missing

  obj->objects = calloc(obj->n_objects, sizeof(gm3Mesh));
  if (!obj->objects) {
    fclose(f);
    return -3;
  }

  fseek(f, 0, SEEK_SET);
  gm3ObjContext ctx = {0};
  for (size_t i = 0; i < obj->n_objects; i++) {
    if (gm3_obj_load_mesh(&obj->objects[i], f, &ctx) < 0) {
      gm3_free_obj(obj);
      fclose(f);
      return -4;
    }
  }
  gm3_center_obj_vertices(obj);

  fclose(f);
  return 0;
}
