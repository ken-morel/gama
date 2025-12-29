#pragma once

#include "position.h"

typedef struct {
  size_t vertices[3];
  size_t material;
  size_t material_file;
  gm3Pos normal;
} gm3MeshFace;

typedef struct {
  gm3Pos *vertices;
  size_t n_vertices;

  gm3MeshFace *faces;
  size_t n_faces;
} gm3Mesh;
