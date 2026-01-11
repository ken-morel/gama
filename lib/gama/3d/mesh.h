#pragma once

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
  double u, v;
} gm3Tex;

typedef struct {
  gm3Pos *vertices;
  size_t n_vertices;

  gm3MeshFace *faces;
  size_t n_faces;

  gm3Pos *normals;
  size_t n_normals;

  gm3Tex *texs;
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

int gm3_mesh_serialize(const gm3Mesh *mesh, void **data, size_t *size);
int gm3_mesh_deserialize(gm3Mesh *mesh, const void *data, size_t size);

// --- Serialization ---
// A simple binary format for baking:
// Header | vertices | faces | tex coords | mtllibs (and their materials) |
// textures
typedef struct {
  uint32_t magic;
  size_t n_vertices;
  size_t n_faces;
  size_t n_normals;
  size_t n_texs;
  size_t n_mtllibs;
} gm3BakedMeshHeader;
#define GM3_BAKED_MESH_MAGIC 0x474D334D // "GM3M"

int gm3_mesh_serialize(const gm3Mesh *mesh, void **data, size_t *size) {
  // Calculate total size for all textures and their metadata
  size_t total_textures_size = 0;
  for (size_t i = 0; i < mesh->n_mtllibs; i++) {
    gm3MtlLib *lib = &mesh->mtllibs[i];
    total_textures_size += sizeof(size_t); // n_textures
    for (size_t j = 0; j < lib->n_textures; j++) {
      gm3Texture *tex = &lib->textures[j];
      size_t data_size = tex->data.width * tex->data.height * 4;
      total_textures_size +=
          sizeof(gmImageData); // width, height (data pointer is not saved)
      total_textures_size += sizeof(size_t); // size of pixel data
      total_textures_size += data_size;      // pixel data itself
      total_textures_size += 256;            // path
    }
  }

  // Calculate total size for all material libraries and their materials
  size_t mtllibs_size = 0;
  for (size_t i = 0; i < mesh->n_mtllibs; i++) {
    mtllibs_size += 256;            // name
    mtllibs_size += sizeof(size_t); // n_materials
    mtllibs_size += sizeof(size_t); // n_textures
    mtllibs_size += sizeof(gm3Material) * mesh->mtllibs[i].n_materials;
  }

  *size = sizeof(gm3BakedMeshHeader) + sizeof(gm3Pos) * mesh->n_vertices +
          sizeof(gm3MeshFace) * mesh->n_faces +
          sizeof(gm3Pos) * mesh->n_normals + sizeof(gm3Tex) * mesh->n_texs +
          mtllibs_size + total_textures_size;

  *data = malloc(*size);
  if (!*data)
    return -1;

  char *p = (char *)*data;

  // Header
  gm3BakedMeshHeader header = {
      .magic = GM3_BAKED_MESH_MAGIC,
      .n_vertices = mesh->n_vertices,
      .n_faces = mesh->n_faces,
      .n_normals = mesh->n_normals,
      .n_texs = mesh->n_texs,
      .n_mtllibs = mesh->n_mtllibs,
  };
  memcpy(p, &header, sizeof(header));
  p += sizeof(header);

  // Main data blocks
  if (mesh->n_vertices > 0) {
    memcpy(p, mesh->vertices, sizeof(gm3Pos) * mesh->n_vertices);
    p += sizeof(gm3Pos) * mesh->n_vertices;
  }
  if (mesh->n_faces > 0) {
    memcpy(p, mesh->faces, sizeof(gm3MeshFace) * mesh->n_faces);
    p += sizeof(gm3MeshFace) * mesh->n_faces;
  }
  if (mesh->n_normals > 0) {
    memcpy(p, mesh->normals, sizeof(gm3Pos) * mesh->n_normals);
    p += sizeof(gm3Pos) * mesh->n_normals;
  }
  if (mesh->n_texs > 0) {
    memcpy(p, mesh->texs, sizeof(gm3Tex) * mesh->n_texs);
    p += sizeof(gm3Tex) * mesh->n_texs;
  }

  // Material Libs and Textures
  for (size_t i = 0; i < mesh->n_mtllibs; i++) {
    gm3MtlLib *lib = &mesh->mtllibs[i];
    // Lib metadata
    memcpy(p, lib->name, 256);
    p += 256;
    memcpy(p, &lib->n_materials, sizeof(size_t));
    p += sizeof(size_t);
    memcpy(p, &lib->n_textures, sizeof(size_t));
    p += sizeof(size_t);

    // Materials
    if (lib->n_materials > 0) {
      memcpy(p, lib->materials, sizeof(gm3Material) * lib->n_materials);
      p += sizeof(gm3Material) * lib->n_materials;
    }

    // Textures
    for (size_t j = 0; j < lib->n_textures; j++) {
      gm3Texture *tex = &lib->textures[j];
      size_t data_size = tex->data.width * tex->data.height * 4;
      memcpy(p, tex->path, 256);
      p += 256;
      memcpy(p, &tex->data.width, sizeof(int32_t));
      p += sizeof(int32_t);
      memcpy(p, &tex->data.height, sizeof(int32_t));
      p += sizeof(int32_t);
      memcpy(p, &data_size, sizeof(size_t));
      p += sizeof(size_t);
      memcpy(p, tex->data.data, data_size);
      p += data_size;
    }
  }

  return 0;
}

int gm3_mesh_deserialize(gm3Mesh *mesh, const void *data, size_t size) {
  memset(mesh, 0, sizeof(gm3Mesh));
  const char *p = (const char *)data;

  if (size < sizeof(gm3BakedMeshHeader))
    return -1;
  const gm3BakedMeshHeader *header = (const gm3BakedMeshHeader *)p;
  if (header->magic != GM3_BAKED_MESH_MAGIC)
    return -1;
  p += sizeof(gm3BakedMeshHeader);

  // Allocate and copy main data blocks
  mesh->n_vertices = header->n_vertices;
  if (mesh->n_vertices > 0) {
    mesh->vertices = malloc(sizeof(gm3Pos) * mesh->n_vertices);
    memcpy(mesh->vertices, p, sizeof(gm3Pos) * mesh->n_vertices);
    p += sizeof(gm3Pos) * mesh->n_vertices;
  }

  mesh->n_faces = header->n_faces;
  if (mesh->n_faces > 0) {
    mesh->faces = malloc(sizeof(gm3MeshFace) * mesh->n_faces);
    memcpy(mesh->faces, p, sizeof(gm3MeshFace) * mesh->n_faces);
    p += sizeof(gm3MeshFace) * mesh->n_faces;
  }

  mesh->n_normals = header->n_normals;
  if (mesh->n_normals > 0) {
    mesh->normals = malloc(sizeof(gm3Pos) * mesh->n_normals);
    memcpy(mesh->normals, p, sizeof(gm3Pos) * mesh->n_normals);
    p += sizeof(gm3Pos) * mesh->n_normals;
  }

  mesh->n_texs = header->n_texs;
  if (mesh->n_texs > 0) {
    mesh->texs = malloc(sizeof(gm3Tex) * mesh->n_texs);
    memcpy(mesh->texs, p, sizeof(gm3Tex) * mesh->n_texs);
    p += sizeof(gm3Tex) * mesh->n_texs;
  }

  // Allocate and copy material libs and textures
  mesh->n_mtllibs = header->n_mtllibs;
  if (mesh->n_mtllibs > 0) {
    mesh->mtllibs = calloc(mesh->n_mtllibs, sizeof(gm3MtlLib));
    for (size_t i = 0; i < mesh->n_mtllibs; i++) {
      gm3MtlLib *lib = &mesh->mtllibs[i];
      memcpy(lib->name, p, 256);
      p += 256;
      memcpy(&lib->n_materials, p, sizeof(size_t));
      p += sizeof(size_t);
      memcpy(&lib->n_textures, p, sizeof(size_t));
      p += sizeof(size_t);

      if (lib->n_materials > 0) {
        lib->materials = malloc(sizeof(gm3Material) * lib->n_materials);
        memcpy(lib->materials, p, sizeof(gm3Material) * lib->n_materials);
        p += sizeof(gm3Material) * lib->n_materials;
      }

      if (lib->n_textures > 0) {
        lib->textures = calloc(lib->n_textures, sizeof(gm3Texture));
        for (size_t j = 0; j < lib->n_textures; j++) {
          gm3Texture *tex = &lib->textures[j];
          size_t data_size = 0;
          memcpy(tex->path, p, 256);
          p += 256;
          memcpy(&tex->data.width, p, sizeof(int32_t));
          p += sizeof(int32_t);
          memcpy(&tex->data.height, p, sizeof(int32_t));
          p += sizeof(int32_t);
          memcpy(&data_size, p, sizeof(size_t));
          p += sizeof(size_t);
          tex->data.data = malloc(data_size);
          memcpy(tex->data.data, p, data_size);
          p += data_size;
        }
      }
    }
  }

  // Check bounds
  if ((size_t)(p - (const char *)data) > size) {
    gm3_mesh_free(mesh);
    return -1; // Data corruption / size mismatch
  }

  return 0;
}
