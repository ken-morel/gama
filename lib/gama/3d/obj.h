#pragma once

#include <ctype.h>
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "mtl.h"

// --- Structures ---

typedef struct {
  gm3Mesh *objects;
  size_t n_objects;

  gm3MtlFile *mtl_files;
  size_t n_mtl_files;
} gm3ObjFile;

typedef struct {
  char type;      // one of: nvfLUog
  char name[128]; // For mtllib and usemtl names
  double points[3];
  long indices[64][3]; // [v, vt, vn]
  size_t n_indices;
} gm3ObjLine;

// --- Internal Helpers ---

static inline void _gm3_u_obj_copy_name(char *dest, const char *src,
                                        size_t max_len) {
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

static void _gm3u_normalize_idx(long *idx, size_t total) {
  if (*idx < 0)
    *idx += (long)total + 1;
}

static long _gm3u_parse_idx(char **ptr) {
  if (!*ptr || isspace((unsigned char)**ptr))
    return 0;
  char *end;
  long val = strtol(*ptr, &end, 10);
  *ptr = end;
  return val;
}

// --- Parsing Logic ---

int gm3_obj_parse_line(char **end, gm3ObjLine *ln) {
  ln->type = '?';
  ln->n_indices = 0;
  memset(ln->name, 0, sizeof(ln->name));

  char *p = skip_ws(*end);
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
  } else if (p[0] == 'v' && p[1] == 't') {
    ln->type = 't';
    p = skip_ws(p + 2);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = 0;
  } else if (p[0] == 'f' && isspace((unsigned char)p[1])) {
    ln->type = 'f';
    p = skip_ws(p + 1);
    while (*p != '\0' && *p != '\n' && ln->n_indices < 64) {
      ln->indices[ln->n_indices][0] = _gm3u_parse_idx(&p);
      if (*p == '/') {
        p++;
        if (*p != '/')
          ln->indices[ln->n_indices][1] = _gm3u_parse_idx(&p);
        if (*p == '/') {
          p++;
          ln->indices[ln->n_indices][2] = _gm3u_parse_idx(&p);
        }
      }
      ln->n_indices++;
      p = skip_ws(p);
    }
  } else if (strncmp(p, "mtllib", 6) == 0) {
    ln->type = 'L';
    _gm3_u_obj_copy_name(ln->name, p + 6, sizeof(ln->name));
  } else if (strncmp(p, "usemtl", 6) == 0) {
    ln->type = 'U';
    _gm3_u_obj_copy_name(ln->name, p + 6, sizeof(ln->name));
  } else if (p[0] == 'o' || p[0] == 'g') {
    ln->type = p[0];
    _gm3_u_obj_copy_name(ln->name, p + 1, sizeof(ln->name));
  }
  while (*p && *p != '\n')
    p++;
  return 0;
}

int gm3_obj_parse(char *content, gm3ObjLine **result, size_t *n_lines) {
  const char *orig = content;
  char *pos = content;
  gm3ObjLine *buffer = malloc(100 * sizeof(gm3ObjLine));
  size_t buffer_size = 100;
  size_t buffer_index = 0;
  gm3ObjLine ln;
  size_t n_vertices = 0, n_txs = 0, n_normals = 0;
  while (gm3_obj_parse_line(&pos, &ln)) {
    for (size_t ind = 0; ind < ln.n_indices; ind++) {
      _gm3u_normalize_idx(&ln.indices[ind][0], n_vertices);
      _gm3u_normalize_idx(&ln.indices[ind][1], n_txs);
      _gm3u_normalize_idx(&ln.indices[ind][2], n_normals);
    }
    switch (ln.type) {
    case 'n':
      n_normals++;
      break;
    case 't':
      n_txs++;
      break;
    case 'v':
      n_vertices++;
      break;
    }
    if (buffer_index >= buffer_size) {
      buffer_size = (size_t)((float)buffer_size * 1.5f);
      gm3ObjLine *newbuff = realloc(buffer, buffer_size * sizeof(gm3ObjLine));
      if (newbuff == NULL) {
        free(buffer);
        return -6;
      }
      buffer[buffer_index++] = ln;
    }
  }
  gm3ObjLine *newbuff = realloc(buffer, buffer_index * sizeof(gm3ObjLine));
  if (!newbuff)
    newbuff = buffer;
  *n_lines = buffer_index;
  *result = buffer;
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
    }
    free(obj->objects);
  }
  // Free material files
  if (obj->mtl_files) {
    for (size_t i = 0; i < obj->n_mtl_files; i++) {
      gm3_mtl_free(obj->mtl_files);
    }
    free(obj->mtl_files);
  }
  memset(obj, 0, sizeof(gm3ObjFile));
}

// --- Material Helper ---

// static size_t gm3_obj_get_material_id(gm3ObjFile *obj, const char *name) {
//   // This is a simple implementation:
//   // We return a global index or pointer. For now, let's assume we store
//   // the index as (mtl_file_index << 16 | material_index_inside_file)
//   for (size_t i = 0; i < obj->n_mtl_files; i++) {
//     for (size_t j = 0; j < obj->mtl_files[i].n_materials; j++) {
//       if (strcmp(obj->mtl_files[i].materials[j].name, name) == 0) {
//         return (i << 16) | j;
//       }
//     }
//   }
//   return 0; // Default material
// }

// --- High Level Loading ---

int gm3_obj_load_object(gm3ObjFile *obj, gm3Mesh *m, gm3ObjLine *parsed,
                        size_t *index, size_t n_parsed) {
  size_t local_v = 0, local_f = 0;
  gm3ObjLine *ln;
  // count data
  memset(m, 0, sizeof(*m));

  for (size_t i = *index; i < n_parsed; i++) {
    ln = &parsed[i];
    if (ln->type == 'o' && local_v > 0)
      break;
    if (ln->type == 'v') {
      local_v++;
    }
    if (ln->type == 'f')
      local_f += (ln->n_indices >= 3 ? ln->n_indices - 2 : 0);
  }

  if (local_v == 0)
    return 0;
  // alocate space

  m->n_vertices = local_v;
  m->n_faces = local_f;
  m->vertices = calloc(local_v, sizeof(gm3Pos));
  m->faces = calloc(local_f, sizeof(gm3MeshFace));

  // load the data
  size_t cv = 0, cf = 0; // current vector and face

  size_t active_mat_file = 0;
  size_t active_mat = 0;

  for (; *index < n_parsed; (*index)++) {
    ln = &parsed[*index];
    if (ln->type == 'o' && cv > 0)
      break;

    if (ln->type == 'L') { // mtllib
      for (size_t i = 0; i < obj->n_mtl_files; i++)
        if (strcmp(obj->mtl_files[i].name, ln->name) == 0) {
          active_mat_file = i + 1;
          break;
        }
    } else if (ln->type == 'U') { // usemtl
      gm3_mtl_find(&obj->mtl_files[active_mat_file - 1], ln->name, &active_mat);
    } else if (ln->type == 'v') {
      m->vertices[cv++] = (gm3Pos){ln->points[0], ln->points[1], ln->points[2]};
    } else if (ln->type == 'f') {
      for (size_t i = 0; i < ln->n_indices - 2; i++) {
        gm3MeshFace *face = &m->faces[cf];
        face->vertices[0] = ln->indices[0][0] - 1;
        face->vertices[1] = ln->indices[i + 1][0] - 1;
        face->vertices[2] = ln->indices[i + 2][0] - 1;

        face->material_file = active_mat_file;
        face->material = active_mat;

        // Calculate normal
        gm3Pos e1 = m->vertices[face->vertices[1]];
        gm3_pos_substract(&e1, &m->vertices[face->vertices[0]]);
        gm3Pos e2 = m->vertices[face->vertices[2]];
        gm3_pos_substract(&e2, &m->vertices[face->vertices[0]]);
        gm3Pos n;
        gm3_pos_cross(&n, &e1, &e2);
        gm3_pos_normalize(&n);

        face->normal = n;
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
      gm3_pos_substract(&obj->objects[i].vertices[v], &center);
  return 0;
}

int gm3_obj_parse_file(const char *path, gm3ObjLine **lines, size_t *n_lines) {
  char *content = NULL;
  int ret;
  ret = gmu_read_file(path, &content, NULL);
  if (ret < 0)
    return ret;
  return gm3_obj_parse(content, lines, n_lines);
}

int gm3_load_obj(gm3ObjFile *obj, const char *path, const char *dir) {
  int ret;
  gm3ObjLine *parsed;
  size_t n_lines;
  memset(obj, 0, sizeof(gm3ObjFile));
  ret = gm3_obj_parse_file(path, &parsed, &n_lines);
  if (ret < 0)
    return ret;

  // load all the object materials

  for (size_t i = 0; i < n_lines; i++) {
    if (parsed[i].type == 'L') {
      gm3MtlFile mf;
      memset(&mf, 0, sizeof(mf));
      char path[2048];
      snprintf(path, sizeof(path), "%s/%s", dir, parsed[i].name);

      if (gm3_mtl_load(&mf, path) >= 0) {
        obj->n_mtl_files++;
        obj->mtl_files = // there may just be a few loads per object
            realloc(obj->mtl_files, sizeof(gm3MtlFile *) * obj->n_mtl_files);
        obj->mtl_files[obj->n_mtl_files - 1] = mf; // it's not so heavy, copy
      }
    } else if (parsed[i].type == 'o') {
      obj->n_objects++;
    }
  }
  // TODO: Default to one object when no object specified
  // if (obj->n_objects == 0)
  //   obj->n_objects = 1;

  obj->objects = calloc(obj->n_objects, sizeof(gm3Mesh));

  size_t index = 0;
  for (size_t i = 0; i < obj->n_objects; i++) {
    while (index < n_lines && parsed[index].type != 'o')
      index++;
    if (0 >
        gm3_obj_load_object(obj, &obj->objects[i], parsed, &index, n_lines)) {
      gm3_free_obj(obj);
      return -4;
    }
  }
  gm3_center_obj_vertices(obj);
  return 0;
}
