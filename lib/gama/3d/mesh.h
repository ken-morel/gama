#include "position.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct {
  size_t vertices[3];
  size_t normal;
} gm3MeshFace;

typedef struct {
  gm3Pos *vertices;
  size_t n_vertices;

  gm3Pos *normals;
  gm3MeshFace *faces;

  size_t n_faces;
} gm3Mesh;
