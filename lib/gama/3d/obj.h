#pragma once

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "mtl.h"

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

  // Collection of loaded material files
  gm3MtlFile **mtl_files;
  size_t n_mtl_files;
} gm3ObjFile;

typedef struct {
  char type;
  char name[128]; // For mtllib and usemtl names
  double points[3];
  long indices[64][3]; // [v, vt, vn]
  size_t n_indices;
} gm3ObjLine;

// --- Internal Helpers ---

static inline void copy_name(char *dest, const char *src, size_t max_len) {
  while (*src && isspace((unsigned char)*src))
    src++; // Skip leading space
  size_t i = 0;
  while (src[i] != '\0' && src[i] != '\n' && src[i] != '\r' &&
         i < max_len - 1) {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
  // Trim trailing spaces
  while (i > 0 && isspace((unsigned char)dest[i - 1])) {
    dest[--i] = '\0';
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
  memset(ln->name, 0, sizeof(ln->name));

  if (!fgets(buf, sizeof(buf), f))
    return -2;

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
  } else if (strncmp(p, "mtllib", 6) == 0) {
    ln->type = 'L';
    copy_name(ln->name, p + 6, sizeof(ln->name));
  } else if (strncmp(p, "usemtl", 6) == 0) {
    ln->type = 'U';
    copy_name(ln->name, p + 6, sizeof(ln->name));
  } else if (p[0] == 'o' || p[0] == 'g') {
    ln->type = p[0];
    copy_name(ln->name, p + 1, sizeof(ln->name));
  }
  return 0;
}

// --- Memory Management ---

void gm3_free_obj(gm3ObjFile *obj) {
  if (!obj)
    return;
  if (obj->objects) {
    for (size_t i = 0; i < obj->n_objects; i++) {
      if (obj->objects[i].vertices)
        free(obj->objects[i].vertices);
      if (obj->objects[i].faces)
        free(obj->objects[i].faces);
      if (obj->objects[i].normals)
        free(obj->objects[i].normals);
    }
    free(obj->objects);
  }
  // Free material files
  if (obj->mtl_files) {
    for (size_t i = 0; i < obj->n_mtl_files; i++) {
      gm3_mtl_free(obj->mtl_files[i]);
    }
    free(obj->mtl_files);
  }
  memset(obj, 0, sizeof(gm3ObjFile));
}

// --- Material Helper ---

static size_t gm3_obj_get_material_id(gm3ObjFile *obj, const char *name) {
  // This is a simple implementation:
  // We return a global index or pointer. For now, let's assume we store
  // the index as (mtl_file_index << 16 | material_index_inside_file)
  for (size_t i = 0; i < obj->n_mtl_files; i++) {
    for (size_t j = 0; j < obj->mtl_files[i]->n_materials; j++) {
      if (strcmp(obj->mtl_files[i]->materials[j].name, name) == 0) {
        return (i << 16) | j;
      }
    }
  }
  return 0; // Default material
}

// --- High Level Loading ---

int gm3_obj_load_mesh(gm3Mesh *m, FILE *f, gm3ObjContext *ctx,
                      gm3ObjFile *parent, const char *dir) {
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
    return 0;

  m->n_vertices = local_v;
  m->n_faces = local_f;
  m->vertices = calloc(local_v, sizeof(gm3Pos));
  m->faces = calloc(local_f, sizeof(gm3MeshFace));
  m->normals = calloc(local_f, sizeof(gm3Pos));

  // Pass 2: Data Load
  fseek(f, mesh_start_pos, SEEK_SET);
  size_t cv = 0, cf = 0;
  size_t v_offset = ctx->total_v - local_v;
  size_t active_mat_idx = 0;

  while (gm3_obj_read_line(f, &ln, ctx) == 0) {
    if (ln.type == 'o' && cv > 0)
      break;

    if (ln.type == 'L') { // mtllib
      // Check if already loaded
      int loaded = 0;
      for (size_t i = 0; i < parent->n_mtl_files; i++)
        if (strcmp(parent->mtl_files[i]->path, ln.name) == 0) {
          loaded = 1;
          break;
        }

      if (!loaded) {
        gm3MtlFile *mf = calloc(1, sizeof(gm3MtlFile));
        char path[2048];
        snprintf(path, sizeof(path), "%s/%s", dir, ln.name);

        if (gm3_mtl_load(mf, path) >= 0) {
          parent->n_mtl_files++;
          parent->mtl_files = realloc(
              parent->mtl_files, sizeof(gm3MtlFile *) * parent->n_mtl_files);
          parent->mtl_files[parent->n_mtl_files - 1] = mf;
        }
      }
    } else if (ln.type == 'U') { // usemtl
      active_mat_idx = gm3_obj_get_material_id(parent, ln.name);
    } else if (ln.type == 'v') {
      m->vertices[cv++] = (gm3Pos){ln.points[0], ln.points[1], ln.points[2]};
    } else if (ln.type == 'f') {
      for (size_t i = 0; i < ln.n_indices - 2; i++) {
        gm3MeshFace *face = &m->faces[cf];
        face->vertices[0] = ln.indices[0][0] - v_offset - 1;
        face->vertices[1] = ln.indices[i + 1][0] - v_offset - 1;
        face->vertices[2] = ln.indices[i + 2][0] - v_offset - 1;
        face->material_idx = active_mat_idx; // Associate material

        // Calculate normal
        gm3Pos e1, e2;
        e1 = gm3_pos_minus(m->vertices[face->vertices[1]],
                           m->vertices[face->vertices[0]]);
        e2 = gm3_pos_minus(m->vertices[face->vertices[2]],
                           m->vertices[face->vertices[0]]);
        gm3Pos n = gm3_pos_cross(e1, e2);
        n = gm3_pos_normalize(n);

        // Deduplication
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
  return 0;
}

int gm3_center_obj_vertices(gm3ObjFile *obj) {
  if (!obj || obj->n_objects == 0)
    return -1;
  double min_p[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
  double max_p[3] = {-DBL_MAX, -DBL_MAX, -DBL_MAX};

  for (size_t i = 0; i < obj->n_objects; i++) {
    for (size_t v = 0; v < obj->objects[i].n_vertices; v++) {
      gm3Pos p = obj->objects[i].vertices[v];
      if (p.x < min_p[0])
        min_p[0] = p.x;
      if (p.x > max_p[0])
        max_p[0] = p.x;
      if (p.y < min_p[1])
        min_p[1] = p.y;
      if (p.y > max_p[1])
        max_p[1] = p.y;
      if (p.z < min_p[2])
        min_p[2] = p.z;
      if (p.z > max_p[2])
        max_p[2] = p.z;
    }
  }

  gm3Pos center = {(min_p[0] + max_p[0]) / 2, (min_p[1] + max_p[1]) / 2,
                   (min_p[2] + max_p[2]) / 2};

  for (size_t i = 0; i < obj->n_objects; i++)
    for (size_t v = 0; v < obj->objects[i].n_vertices; v++)
      obj->objects[i].vertices[v] =
          gm3_pos_minus(obj->objects[i].vertices[v], center);

  return 0;
}

int gm3_load_obj(gm3ObjFile *obj, const char *path, const char *dir) {
  FILE *f = fopen(path, "r");
  if (!f)
    return -1;

  memset(obj, 0, sizeof(gm3ObjFile));
  snprintf(obj->path, sizeof(obj->path), "%s", path);

  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (line[0] == 'o' && isspace((unsigned char)line[1]))
      obj->n_objects++;
  }
  if (obj->n_objects == 0)
    obj->n_objects = 1;

  obj->objects = calloc(obj->n_objects, sizeof(gm3Mesh));
  fseek(f, 0, SEEK_SET);
  gm3ObjContext ctx = {0};
  for (size_t i = 0; i < obj->n_objects; i++) {
    if (gm3_obj_load_mesh(&obj->objects[i], f, &ctx, obj, dir) < 0) {
      gm3_free_obj(obj);
      fclose(f);
      return -4;
    }
  }
  gm3_center_obj_vertices(obj);
  fclose(f);
  return 0;
}
