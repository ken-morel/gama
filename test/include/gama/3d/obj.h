#pragma once

#include <ctype.h>
#include <float.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "mesh.h"
#include "mtl.h"

// --- Structures ---

typedef struct {
  char type;      // one of: nvfLUog
  char name[128]; // For mtllib and usemtl names
  double points[3];
  long long indices[64][3]; // [v, vt, vn]
  size_t n_indices;
} gm3ObjLine;

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

static inline char *gmu_skip_ws(char *s) {
  for (; *s && *s == ' '; s++)
    ;
  return s;
}

static long long _gm3u_parse_idx(char **ptr) {
  if (!ptr || !*ptr || isspace((unsigned char)**ptr))
    return 0;
  char *end;
  long long val = strtoll(*ptr, &end, 10);
  *ptr = end;
  return val;
}

// --- Parsing Logic ---

int gm3_obj_parse_next_line(char **end, gm3ObjLine *ln) {
  memset(ln, 0, sizeof(*ln));
  ln->type = '?';

  char *p = gmu_skip_ws(*end);
  if (*p == '\0')
    return 0;

  if (p[0] == 'v' && isspace((unsigned char)p[1])) {
    ln->type = 'v';
    p = gmu_skip_ws(p + 1);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = strtod(p, &p);
  } else if (p[0] == 'v' && p[1] == 'n') {
    ln->type = 'n';
    p = gmu_skip_ws(p + 2);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = strtod(p, &p);
  } else if (p[0] == 'v' && p[1] == 't') {
    ln->type = 't';
    p = gmu_skip_ws(p + 2);
    ln->points[0] = strtod(p, &p);
    ln->points[1] = strtod(p, &p);
    ln->points[2] = 0;
  } else if (p[0] == 'f' && isspace((unsigned char)p[1])) {
    ln->type = 'f';
    p = gmu_skip_ws(p + 1);
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
      p = gmu_skip_ws(p);
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
  } else if (p[0] == '#') {
    ln->type = '#';
  }

  // Advance to next line
  while (*p && *p != '\n')
    p++;
  if (*p == '\n')
    p++;
  *end = p;
  return 1;
}
static void _gm3u_normalize_idx(long long *idx, size_t total) {
  if (*idx < 0)
    *idx += (long long)total;
  else
    *idx -= 1;
}
int gm3_obj_parse(char *content, gm3ObjLine **result, size_t *n_lines) {
  char *pos = content;
  size_t buffer_size = 100;
  size_t buffer_index = 0;

  gm3ObjLine *buffer = malloc(buffer_size * sizeof(gm3ObjLine));
  if (!buffer)
    return -1;

  gm3ObjLine ln;
  size_t n_vertices = 0, n_txs = 0, n_normals = 0;

  while (gm3_obj_parse_next_line(&pos, &ln)) {
    if (ln.type == 'f') {
      for (size_t ind = 0; ind < ln.n_indices; ind++) {

        _gm3u_normalize_idx(&ln.indices[ind][0], n_vertices);
        _gm3u_normalize_idx(&ln.indices[ind][1], n_txs);
        _gm3u_normalize_idx(&ln.indices[ind][2], n_normals);
        // okay here
      }
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
      size_t new_size = (size_t)((float)buffer_size * 1.5f);
      gm3ObjLine *newbuff = realloc(buffer, new_size * sizeof(gm3ObjLine));
      if (newbuff == NULL) {
        free(buffer);
        return -6;
      }
      buffer = newbuff;
      buffer_size = new_size;
    }
    buffer[buffer_index++] = ln;
  }

  // Trim buffer
  gm3ObjLine *final_buff = realloc(buffer, buffer_index * sizeof(gm3ObjLine));
  if (final_buff)
    buffer = final_buff;

  *n_lines = buffer_index;
  *result = buffer;
  return 0;
}

// --- High Level Loading ---

int gm3_obj_parse_file(const char *path, gm3ObjLine **lines, size_t *n_lines) {
  char *content = NULL;
  int ret;
  // Assuming gmu_read_file is defined elsewhere
  ret = gmu_read_file(path, &content, NULL);
  if (ret < 0)
    return ret;
  ret = gm3_obj_parse(content, lines, n_lines);
  free(content);
  return ret;
}

int gm3_obj_load(gm3Mesh *m, const char *path, const char *dir) {
  int ret;
  gm3ObjLine *parsed;
  size_t n_lines;
  memset(m, 0, sizeof(*m));

  ret = gm3_obj_parse_file(path, &parsed, &n_lines);
  if (ret < 0)
    return ret;

  for (size_t i = 0; i < n_lines; i++) {
    if (parsed[i].type == 'L') {
      gm3MtlLib mf;
      memset(&mf, 0, sizeof(mf));
      char mtl_path[2048];
      snprintf(mtl_path, sizeof(mtl_path), "%s/%s", dir, parsed[i].name);

      if (gm3_mtl_load(&mf, mtl_path) >= 0) {
        m->n_mtllibs++;
        gm3MtlLib *tmp = realloc(m->mtllibs, sizeof(gm3MtlLib) * m->n_mtllibs);
        if (!tmp) {
          free(parsed);
          gm3_mesh_free(m);
          return -5;
        }
        m->mtllibs = tmp;
        m->mtllibs[m->n_mtllibs - 1] = mf;
      }
    }
  }

  size_t total_vertices = 0, total_faces = 0, total_normals = 0, total_texs = 0;

  // 1. Pass: Count vertices and faces for this object block
  for (size_t i = 0; i < n_lines; i++) {
    if (parsed[i].type == 'v')
      total_vertices++;
    else if (parsed[i].type == 'f') {
      total_faces += parsed[i].n_indices >= 3 ? parsed[i].n_indices - 1 : 0;
    } else if (parsed[i].type == 'n')
      total_normals++;
    else if (parsed[i].type == 't')
      total_texs++;
  }

  m->n_vertices = total_vertices;
  m->n_faces = total_faces;
  m->n_normals = total_normals;
  m->n_texs = total_texs;

  if (total_vertices > 0)
    m->vertices = calloc(total_vertices, sizeof(gm3Pos));
  if (total_faces > 0)
    m->faces = calloc(total_faces, sizeof(gm3MeshFace));
  if (total_normals > 0)
    m->normals = calloc(total_normals, sizeof(gm3Pos));
  if (total_texs > 0)
    m->texs = calloc(total_texs, sizeof(gmPos));

  // 2. Pass: Load Data
  size_t cv = 0, cf = 0, cn = 0, ct = 0;
  int active_mat_file = 0;
  int active_mat = 0;
  gm3ObjLine *ln;
  for (size_t index = 0; index < n_lines; index++) {
    ln = &parsed[index];
    if (ln->type == 'L') { // mtllib
      for (size_t i = 0; i < m->n_mtllibs; i++)
        if (strcmp(m->mtllibs[i].name, ln->name) == 0) {
          active_mat_file = i + 1; // Store as 1-based for safety
          break;
        }
    } else if (ln->type == 'U') { // usemtl
      if (active_mat_file > 0)
        gm3_mtl_find_mat(&m->mtllibs[active_mat_file - 1], ln->name,
                         &active_mat);
    } else if (ln->type == 'n') {
      m->normals[cn].x = ln->points[0];
      m->normals[cn].y = ln->points[1];
      m->normals[cn].z = ln->points[2];
      cn++;
    } else if (ln->type == 'v') {
      if (cv < m->n_vertices) {
        m->vertices[cv++] =
            (gm3Pos){ln->points[0], ln->points[1], ln->points[2]};
        gmdn(pos3, m->vertices[cv - 1]);
        printf("\n");
      }

    } else if (ln->type == 't') {
      m->texs[ct].x = ln->points[0];
      m->texs[ct].y = ln->points[1];
      ct++;
    } else if (ln->type == 'f') {
      for (size_t i = 0; i < ln->n_indices - 2; i++) {
        if (cf >= m->n_faces)
          break;

        gm3MeshFace *face = &m->faces[cf];

        long idx0 = ln->indices[0][0];
        long idx1 = ln->indices[i + 1][0];
        long idx2 = ln->indices[i + 2][0];

        face->texs[0] = ln->indices[0][1];
        face->texs[1] = ln->indices[i + 1][1];
        face->texs[2] = ln->indices[i + 2][1];

        // Bounds check to prevent segfaults on malformed files
        if (idx0 < 0 || idx0 >= (long)m->n_vertices)
          idx0 = 0;
        if (idx1 < 0 || idx1 >= (long)m->n_vertices)
          idx1 = 0;
        if (idx2 < 0 || idx2 >= (long)m->n_vertices)
          idx2 = 0;

        face->vertices[0] = (size_t)idx0;
        face->vertices[1] = (size_t)idx1;
        face->vertices[2] = (size_t)idx2;

        face->material_file = active_mat_file;
        face->material = active_mat;

        // Calculate normal
        gm3Pos e1 = m->vertices[face->vertices[1]];
        gm3_pos_substract(&e1, &m->vertices[face->vertices[0]]);
        gm3Pos e2 = m->vertices[face->vertices[2]];
        gm3_pos_substract(&e2, &m->vertices[face->vertices[0]]);
        gm3Pos n = {0};

        if (ln->indices[0][2] >=
            0) { // find the normal's orientation by comparing one of the faces
                 // vertex's normals
          gm3_pos_cross(&n, &e1, &e2);
          gm3_pos_normalize(&n);
          double d = gm3_pos_dot(&n, &m->normals[ln->indices[0][2]]);
          if (d < 0)
            gm3_pos_mul_scalar(&n, -1);
        }

        face->normal = n;
        cf++;
      }
    }
  }

  gm3_mesh_center(m);

  free(parsed); // Clean up the line buffer
  return 0;
}
