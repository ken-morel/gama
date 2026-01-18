/**
 * @file obj.h
 * @brief Implements a Wavefront OBJ 3D model file loader.
 *
 * This file provides functionality to parse .obj files and load their
 * geometric and material data into a `gm3Mesh` structure. It also handles
 * the parsing of associated .mtl files indirectly via `mtl.h`.
 */
#pragma once

#include <ctype.h> // For isspace, isalpha
#include <float.h> // For DBL_MAX
#include <stddef.h> // For size_t
#include <stdint.h> // For int32_t
#include <stdio.h> // For snprintf
#include <stdlib.h> // For malloc, realloc, free, strtod, strtoll
#include <string.h> // For strcmp, strncmp, memset, strlen

#include "mesh.h"
#include "mtl.h"
#include "position.h"

// --- Structures ---

/**
 * @internal
 * @brief Represents a single parsed line from an OBJ file.
 */
typedef struct {
  char type;      /**< The type of OBJ line (e.g., 'v', 'f', 'L' for mtllib, 'U' for usemtl). */
  char name[128]; /**< For mtllib and usemtl names. */
  double points[3]; /**< Stores vertex/normal/texcoord data. */
  long long indices[64][3]; /**< Stores face indices [v, vt, vn] for up to 64 vertices. */
  size_t n_indices; /**< Number of indices parsed for a face. */
} gm3ObjLine;

/**
 * @internal
 * @brief Skips leading whitespace characters in a string.
 * @param s The string to process.
 * @return A pointer to the first non-whitespace character in the string.
 */
static inline char *gmu_skip_ws(char *s) {
  while (*s && *s == ' ')
    s++;
  return s;
}

/**
 * @internal
 * @brief Parses a single index value from an OBJ face line.
 * @param ptr A pointer to the current position in the line string.
 * @return The parsed index, or 0 if no valid index is found.
 */
static long long _gm3u_parse_idx(char **ptr) {
  if (!ptr || !*ptr || isspace((unsigned char)**ptr) || **ptr == '/')
    return 0;
  char *end;
  long long val = strtoll(*ptr, &end, 10);
  *ptr = end;
  return val;
}

// --- Parsing Logic ---

/**
 * @internal
 * @brief Parses the next line from an OBJ file content string.
 *
 * This function identifies the type of OBJ line (vertex, face, normal,
 * texture coordinate, material library, use material, object/group)
 * and extracts its relevant data into a `gm3ObjLine` struct.
 *
 * @param end A pointer to the current read position in the OBJ content string.
 *        This will be updated to point to the start of the next line.
 * @param ln A pointer to the `gm3ObjLine` struct to populate with parsed data.
 * @return 1 if a line was successfully parsed, 0 if the end of content is reached.
 */
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
  } else if (p[0] == 'f' && isspace((unsigned char)p[1])) {
    ln->type = 'f';
    p = gmu_skip_ws(p + 1);
    while (*p != '\0' && *p != '\n' && *p != '\r' && ln->n_indices < 64) {
      ln->indices[ln->n_indices][0] = _gm3u_parse_idx(&p);
      if (*p == '/') {
        p++;
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
    gm3u_str_copy_eol(ln->name, p + 6, sizeof(ln->name));
  } else if (strncmp(p, "usemtl", 6) == 0) {
    ln->type = 'U';
    gm3u_str_copy_eol(ln->name, p + 6, sizeof(ln->name));
  } else if (p[0] == 'o' || p[0] == 'g') {
    ln->type = p[0];
    gm3u_str_copy_eol(ln->name, p + 1, sizeof(ln->name));
  } else if (p[0] == '#') {
    ln->type = '#';
  }

  while (*p && *p != '\n')
    p++;
  if (*p == '\n')
    p++;
  *end = p;
  return 1;
}

/**
 * @internal
 * @brief Normalizes an index read from an OBJ file.
 *
 * OBJ indices can be 1-based (positive), relative to the end (-negative),
 * or 0 (fallback). This converts them to 0-based absolute indices.
 *
 * @param idx A pointer to the index to normalize.
 * @param total The total number of elements in the corresponding array (e.g., total vertices).
 */
static void _gm3u_normalize_idx(long long *idx, size_t total) {
  if (*idx > 0)
    *idx -= 1;
  else if (*idx < 0)
    *idx += (long long)total;
  else
    *idx = 0; // fallback
}

/**
 * @internal
 * @brief Parses the entire content of an OBJ file into an array of `gm3ObjLine` structs.
 * @param content The null-terminated string content of the OBJ file.
 * @param result A pointer to a `gm3ObjLine**` that will store the parsed lines. This memory
 *        is dynamically allocated and must be freed by the caller.
 * @param n_lines A pointer to a `size_t` that will store the number of parsed lines.
 * @return 0 on success, -1 on memory allocation failure.
 */
int gm3_obj_parse(char *content, gm3ObjLine **result, size_t *n_lines) {
  char *pos = content;
  size_t buffer_size = 128;
  size_t buffer_index = 0;
  gm3ObjLine *buffer = malloc(buffer_size * sizeof(gm3ObjLine));
  if (!buffer)
    return -1;

  gm3ObjLine ln;
  while (gm3_obj_parse_next_line(&pos, &ln)) {
    if (buffer_index >= buffer_size) {
      size_t new_size = buffer_size * 2;
      gm3ObjLine *newbuff = realloc(buffer, new_size * sizeof(gm3ObjLine));
      if (!newbuff) {
        free(buffer);
        return -1;
      }
      buffer = newbuff;
      buffer_size = new_size;
    }
    buffer[buffer_index++] = ln;
  }

  *n_lines = buffer_index;
  *result = buffer;
  return 0;
}

/**
 * @brief Loads a Wavefront OBJ 3D model from a file into a `gm3Mesh` structure.
 *
 * This function parses the `.obj` file, including vertices, normals, texture
 * coordinates, and faces. It also loads associated `.mtl` material libraries.
 * The `gm3Mesh` will be populated with all the data.
 *
 * @param m A pointer to the `gm3Mesh` structure to populate.
 * @param path The file path to the .obj model.
 * @param dir The directory containing the .obj file, used for resolving relative .mtl paths.
 * @return 0 on success, -1 on file reading or parsing failure, or a negative
 *         value from `gm3_mtl_load` on material loading failure.
 */
int32_t gm3_obj_load(gm3Mesh *m, const char *path, const char *dir) {
  char *content = NULL;
  size_t content_len;
  if (gmu_read_file(path, &content, &content_len) < 0)
    return -1;

  gm3ObjLine *parsed;
  size_t n_lines;
  memset(m, 0, sizeof(*m));

  if (gm3_obj_parse(content, &parsed, &n_lines) < 0) {
    free(content);
    return -1;
  }
  free(content);

  // 1. Load Material Libraries
  for (size_t i = 0; i < n_lines; i++) {
    if (parsed[i].type == 'L') {
      char mtl_path[2048];
      snprintf(mtl_path, sizeof(mtl_path), "%s/%s", dir, parsed[i].name);
      gm3MtlLib mf;
      memset(&mf, 0, sizeof(mf));
      int ret = gm3_mtl_load(&mf, mtl_path, dir);
      if (ret < 0)
        return ret;
      if (ret >= 0) {
        gm3MtlLib *tmp =
            realloc(m->mtllibs, sizeof(gm3MtlLib) * (m->n_mtllibs + 1));
        if (tmp) {
          m->mtllibs = tmp;
          m->mtllibs[m->n_mtllibs++] = mf;
        }
      }
    }
  }

  // 2. Pass: Count Totals
  size_t total_v = 0, total_f = 0, total_n = 0, total_t = 0;
  for (size_t i = 0; i < n_lines; i++) {
    if (parsed[i].type == 'v')
      total_v++;
    else if (parsed[i].type == 'n')
      total_n++;
    else if (parsed[i].type == 't')
      total_t++;
    else if (parsed[i].type == 'f' && parsed[i].n_indices >= 3) {
      total_f += (parsed[i].n_indices - 2); // Triangulation
    }
  }

  m->n_vertices = total_v;
  m->n_faces = total_f;
  m->n_normals = total_n;
  m->n_texs = total_t;
  m->vertices = calloc(total_v, sizeof(gm3Pos));
  m->faces = calloc(total_f, sizeof(gm3MeshFace));
  m->normals = calloc(total_n, sizeof(gm3Pos));
  m->texs = calloc(total_t, sizeof(gmPos));

  // 3. Pass: Store Data
  size_t cv = 0, cf = 0, cn = 0, ct = 0;
  int active_mat_file = -1;
  int active_mat = -1;

  for (size_t i = 0; i < n_lines; i++) {
    gm3ObjLine *ln = &parsed[i];
    if (ln->type == 'L') {
      for (size_t j = 0; j < m->n_mtllibs; j++)
        if (strcmp(m->mtllibs[j].name, ln->name) == 0) {
          active_mat_file = (int)j;
          break;
        }
    } else if (ln->type == 'U') {
      if (active_mat_file >= 0)
        gm3_mtl_find_mat(&m->mtllibs[active_mat_file], ln->name, &active_mat);
    } else if (ln->type == 'v') {
      m->vertices[cv++] = (gm3Pos){ln->points[0], ln->points[1], ln->points[2]};
    } else if (ln->type == 'n') {
      m->normals[cn++] = (gm3Pos){ln->points[0], ln->points[1], ln->points[2]};
    } else if (ln->type == 't') {
      m->texs[ct++] = (gm3Tex){ln->points[0], ln->points[1]};
    } else if (ln->type == 'f') {
      // Normalize all indices for this face
      for (size_t j = 0; j < ln->n_indices; j++) {
        _gm3u_normalize_idx(&ln->indices[j][0], total_v);
        _gm3u_normalize_idx(&ln->indices[j][1], total_t);
        _gm3u_normalize_idx(&ln->indices[j][2], total_n);
      }
      // Triangulate (Fan)
      for (size_t j = 0; j < ln->n_indices - 2; j++) {
        gm3MeshFace *face = &m->faces[cf++];
        face->vertices[0] = (size_t)ln->indices[0][0];
        face->vertices[1] = (size_t)ln->indices[j + 1][0];
        face->vertices[2] = (size_t)ln->indices[j + 2][0];
        face->uvs[0] = (size_t)ln->indices[0][1];
        face->uvs[1] = (size_t)ln->indices[j + 1][1];
        face->uvs[2] = (size_t)ln->indices[j + 2][1];
        face->material_file = active_mat_file;
        face->material = active_mat;

        // Normal calculation
        gm3Pos e1 = m->vertices[face->vertices[1]];
        gm3_pos_substract(&e1, &m->vertices[face->vertices[0]]);
        gm3Pos e2 = m->vertices[face->vertices[2]];
        gm3_pos_substract(&e2, &m->vertices[face->vertices[0]]);
        gm3Pos n = gm3_pos_cross(e1, e2);
        gm3_pos_normalize(&n);

        face->normal = n;
      }
    }
  }

  gm3_mesh_center(m);
  free(parsed);
  return 0;
}