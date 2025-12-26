// #include "../_math.h"
#include <ctype.h>
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "position.h"

typedef struct {
  char path[100];
  gm3Mesh *objects;
  size_t n_objects;
} gm3ObjFile;

typedef struct {
  char type;
  char text[20];       // obj, mtl, group
  double points[3];    // for normals & vertices & tex
  long indices[64][3]; // for faces
  size_t n_indices;
} gm3ObjLine;

int gm3_obj_goto_next_object(FILE *f) {
  char c;
  do {
    c = fgetc(f);
    if (c == 'o')
      return 1;
    while (c != '\n' && c != EOF)
      c = fgetc(f);
  } while (c != EOF);
  return 0;
}
int gm3_obj_count_objects(FILE *f, size_t *n) {
  *n = 0;
  for (*n = 0; gm3_obj_goto_next_object(f); (*n)++)
    ;
  return 0;
}

void gm3_obj_reset_line(gm3ObjLine *ln) {
  ln->type = '?';
  for (size_t i = 0; i < sizeof(ln->text); i++)
    ln->text[i] = '\0';
  for (size_t i = 0; i < sizeof(ln->points) / sizeof(ln->points[0]); i++)
    ln->points[i] = 0;
  for (size_t i = 0; i < sizeof(ln->indices) / sizeof(ln->indices[0]); i++)
    for (size_t j = 0; j < sizeof(ln->indices[0]) / sizeof(ln->indices[0][0]);
         j++)
      ln->indices[i][j] = 0;
}

size_t _gm3u_read_until_eol(const char *src, char *dest,
                            const size_t dest_size) {
  if (dest_size == 0)
    return 0;
  while (*src == ' ')
    src++;
  size_t i = 0;
  while (*src != '\0' && *src != '\r' && *src != '\n' && i < dest_size - 1) {
    dest[i++] = *src++;
  } // Truncate trailing whitespace
  while (i > 0 && dest[i - 1] == ' ') {
    i--;
  }
  dest[i] = '\0';
  return i;
}

int gm3_obj_read_line(FILE *f, gm3ObjLine *ln) {
  char line_buffer[4096]; // Much larger buffer for face lines
  size_t line_start;
  gm3_obj_reset_line(ln);
  // Read line
  if (!fgets(line_buffer, sizeof(line_buffer), f))
    return -2; // EOF or error

  // Skip leading whitespace
  line_start = 0;
  while (line_start < strlen(line_buffer) && isspace(line_buffer[line_start]))
    line_start++;

  // Check for empty line or comment
  if (line_buffer[line_start] == '\0' || line_buffer[line_start] == '\n' ||
      line_buffer[line_start] == '\r' || line_buffer[line_start] == '#') {
    if (line_buffer[line_start] == '#') {
      ln->type = '#';
      return 0;
    }
    ln->type = ' ';
    return 0; // Empty line
  }

  // Check for different commands using proper strncmp (returns 0 when equal)
  if (strncmp(&line_buffer[line_start], "v ", 2) == 0) {
    // vertex: v %f %f %f
    double x, y, z;
    int fields = sscanf(&line_buffer[line_start], "v %lf %lf %lf", &x, &y, &z);
    if (fields < 3)
      return -1;
    ln->type = 'v';
    ln->points[0] = x;
    ln->points[1] = y;
    ln->points[2] = z;
    return 0;
  } else if (strncmp(&line_buffer[line_start], "vt ", 3) == 0) {
    // texture: vt %f %f
    double u, v;
    int fields = sscanf(&line_buffer[line_start], "vt %lf %lf", &u, &v);
    if (fields < 2)
      return -1;
    ln->type = 't';
    ln->points[0] = u;
    ln->points[1] = v;
    return 0;
  } else if (strncmp(&line_buffer[line_start], "vn ", 3) == 0) {
    // normal: vn %f %f %f
    double x, y, z;
    int fields = sscanf(&line_buffer[line_start], "vn %lf %lf %lf", &x, &y, &z);
    if (fields < 3)
      return -1;
    ln->type = 'n';
    ln->points[0] = x;
    ln->points[1] = y;
    ln->points[2] = z;
    return 0;
  } else if (strncmp(&line_buffer[line_start], "o ", 2) == 0) {
    // object: o %s
    ln->type = 'o';
    _gm3u_read_until_eol(&line_buffer[line_start + 2], ln->text,
                         sizeof(ln->text));
    return 0;
  } else if (strncmp(&line_buffer[line_start], "g ", 2) == 0) {
    // group: g %s
    ln->type = 'g';
    _gm3u_read_until_eol(&line_buffer[line_start + 2], ln->text,
                         sizeof(ln->text));
    return 0; // Missing return!
  } else if (strncmp(&line_buffer[line_start], "usemtl ", 7) == 0) {
    // usemtl %s
    ln->type = 'm';
    _gm3u_read_until_eol(&line_buffer[line_start + 7], ln->text,
                         sizeof(ln->text));
    return 0; // Missing return!
  } else if (strncmp(&line_buffer[line_start], "f ", 2) == 0) {
    // face parsing - need to use a separate buffer for tokenization
    char face_buffer[4096];
    strcpy(face_buffer, &line_buffer[line_start + 2]);

    char *token;
    char *saveptr; // For strtok_r (thread-safe version)

    // Use strtok_r to avoid nested strtok issues
    token = strtok_r(face_buffer, " \t\n\r", &saveptr);
    int vertex_count = 0;

    while (token != NULL &&
           vertex_count < sizeof(ln->indices) / sizeof(ln->indices[0])) {
      // Parse the vertex/texcoord/normal triple: v/vt/vn
      int v_idx = 0, vt_idx = 0, vn_idx = 0;

      // Create a copy for parsing since strtok modifies the string
      char vertex_spec[256];
      strncpy(vertex_spec, token, sizeof(vertex_spec) - 1);
      vertex_spec[sizeof(vertex_spec) - 1] = '\0';
      // Find the slashes to split the specification
      char *part1 = strtok_r(vertex_spec, "/", &saveptr);
      char *part2 = strtok_r(NULL, "/", &saveptr);
      char *part3 = strtok_r(NULL, "/", &saveptr);
      if (part1 && strlen(part1) > 0) {
        v_idx = atoi(part1);
      }
      if (part2 && strlen(part2) > 0) {
        vt_idx = atoi(part2);
      }
      // Note: part2 could be empty (v//vn format), which is OK
      if (part3 && strlen(part3) > 0) {
        vn_idx = atoi(part3);
      }
      // Store indices (remember to handle negative indices)
      ln->indices[vertex_count][0] = v_idx;
      ln->indices[vertex_count][1] = vt_idx;
      ln->indices[vertex_count][2] = vn_idx;
      vertex_count++;
      // Get next token from original string, not the modified copy
      token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    ln->type = 'f';
    ln->n_indices = vertex_count;
    return 0;
  }
  // Unknown command
  ln->type = '?';
  strncpy(ln->text, &line_buffer[line_start], sizeof(ln->text) - 1);
  ln->text[sizeof(ln->text) - 1] = '\0';
  return 0;
}

int gm3_obj_count_mesh_components(FILE *f, size_t *vertices, size_t *normals,
                                  size_t *faces) {
  *vertices = 0;
  *normals = 0;
  *faces = 0;
  gm3ObjLine ln;
  int ret;
  do {
    ret = gm3_obj_read_line(f, &ln);
    switch (ln.type) {
    case 'f':
      (*faces)++;
      break;
    case 'v':
      (*vertices)++;
      break;
    case 'n':
      (*normals)++;
      break;
    }
  } while (ret >= 0);

  if (ret > 0 || ret == -2)
    return 0;
  else
    return ret;
}
int gm3_obj_load_mesh(gm3Mesh *m, FILE *f, size_t obj_idx) {
  m->n_faces = 0;
  m->faces = NULL;
  m->normals = NULL;
  m->n_vertices = 0;
  m->vertices = NULL;

  int ret;

  size_t i;
  char c;
  for (i = 0; i <= obj_idx; i++)
    if (!gm3_obj_goto_next_object(f))
      return 1;
  do {
    c = fgetc(f);
  } while (c != '\n' && c != EOF);
  if (c == EOF)
    return 2;
  size_t obj_start = ftell(f);

  size_t n_vertex_normals;
  // A face has three normals and three textures for each verex.
  // Buf for gama we will use a single color and single normal
  // for each face, using the average of those

  ret = gm3_obj_count_mesh_components(f, &m->n_vertices, &n_vertex_normals,
                                      &m->n_faces);
  fseek(f, obj_start, SEEK_SET);
  if (ret < 0)
    return ret;
  gm3Pos *vertex_normals = calloc(n_vertex_normals, sizeof(gm3Pos));
  m->faces = calloc(m->n_faces, sizeof(gm3MeshFace));
  m->vertices = calloc(m->n_vertices, sizeof(gm3Pos));

  m->normals = calloc(m->n_faces, sizeof(gm3Pos));

  gm3ObjLine ln;
  ret = gm3_obj_read_line(f, &ln);

  size_t vertex = 0, face = 0, vertex_normal = 0;
  // TODO: In future shrink normals to save memory
  // and prevent repetitions, let paralel faces have the same
  // .normal
  // cache normals for them to be computed latter
  //
  size_t *face_vertices_normals = calloc(m->n_faces * 3, sizeof(size_t));

  while (ret >= 0) {
    switch (ln.type) {
    case 'v':
      if (vertex < m->n_vertices) {
        m->vertices[vertex].x = ln.points[0];
        m->vertices[vertex].y = ln.points[1];
        m->vertices[vertex].z = ln.points[2];
      }
      vertex++;
      break;
    case 'n':
      if (vertex_normal < n_vertex_normals) {
        vertex_normals[vertex_normal].x = ln.points[0];
        vertex_normals[vertex_normal].y = ln.points[1];
        vertex_normals[vertex_normal].z = ln.points[2];
      }
      vertex_normal++;
      break;
    case 'f':
      if (face < m->n_faces && ln.n_indices >= 3) {
        // correct 1 based index
        m->faces[face].vertices[0] = ln.indices[0][0] - 1;
        m->faces[face].vertices[1] = ln.indices[1][0] - 1;
        m->faces[face].vertices[2] = ln.indices[2][0] - 1;

        face_vertices_normals[0 + face * 3] = ln.indices[0][2] - 1;
        face_vertices_normals[1 + face * 3] = ln.indices[0][2] - 1;
        face_vertices_normals[2 + face * 3] = ln.indices[0][2] - 1;
      }
      face++;
      break;
    }
    ret = gm3_obj_read_line(f, &ln);
  }
  if (ret != -2) {
    // TODO: free memory
    return ret;
  }
  // compute face normals
  size_t normal_idx = 0; // the normal index
  size_t fi;             // the face index
  for (fi = 0; fi < m->n_faces && fi < face; fi++) {
    m->normals[normal_idx].x = vertex_normals[fi * 3].x +
                               vertex_normals[1 + fi * 3].x +
                               vertex_normals[2 + fi * 3].x;
    m->normals[normal_idx].x = vertex_normals[fi * 3].x +
                               vertex_normals[1 + fi * 3].x +
                               vertex_normals[2 + fi * 3].x;
    m->normals[normal_idx].x = vertex_normals[fi * 3].x +
                               vertex_normals[1 + fi * 3].x +
                               vertex_normals[2 + fi * 3].x;
    m->faces[fi].normal = normal_idx;
    for (size_t i = 0; i < normal_idx; i++) {
      if (gm3_pos_distance(m->normals[normal_idx], m->normals[i]) < 0.0001) {
        m->faces[fi].normal = i;
      }
    }
    if (m->faces[fi].normal == normal_idx) {
      normal_idx++; // just use this normal then
    }
  }
  free(vertex_normals);
  free(face_vertices_normals);
  return 0;
}

int gm3_load_obj(gm3ObjFile *obj, const char *path) {
  int ret;
  snprintf(obj->path, sizeof(obj->path), "%s", path);

  FILE *f = fopen(path, "r");

  ret = gm3_obj_count_objects(f, &obj->n_objects);
  fseek(f, 0, SEEK_SET);
  if (ret != 0)
    return ret;

  obj->objects = calloc(obj->n_objects, sizeof(gm3Mesh));
  for (size_t obj_index = 0; obj_index < obj->n_objects; obj_index++) {
    int ret = gm3_obj_load_mesh(&obj->objects[obj_index], f, obj_index);
    if (ret < 0)
      return ret;
    fseek(f, 0, SEEK_SET);
  }

  return 0;
}
