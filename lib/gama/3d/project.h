#pragma once

#include "../color.h"
#include "../position.h"
#include "light.h"
#include "obj.h"
#include "position.h"
#include "scene.h"
#include "transform.h"
#include <stddef.h>

typedef struct {
  gmPos *vertices;
  size_t n_vertices;

  gmColor *colors;
  size_t n_colors;

  size_t *triangles;
  size_t n_triangles;
} gm3Image;

typedef struct {
  gmPos vertices[3];
  gmColor color; // Final lit color
  int visible;   // 0 if behind camera or culled
} gm3TriangleImage;

int gm3_project_face(gm3TriangleImage *out, gm3Pos norm, gm3Pos *vertices,
                     gm3Scene *scene) {
  // 1. Perspective Clipping Check
  gm3Pos *_vert = vertices;
  for (size_t i = 0; i < 3; i++) {
    if ((*_vert).z <= scene->near || (*_vert).z > scene->far) {
      out->visible = 0;
      return 1;
    }
    _vert++;
  }

  // 2. Lighting Calculation (Lambertian Diffuse)
  gm3Pos face_center = gm3_pos_center3(vertices[0], vertices[1], vertices[2]);

  // Vector from face to light position
  gm3Pos l_vec = gm3_pos_minus(scene->light.position, face_center);

  // normalize the light vector, 0->1
  l_vec = gm3_pos_normalize(l_vec);

  // Dot product: Normal · LightVector
  // This gives the cosine of the angle between them (0.0 to 1.0)
  // so cool
  double dot = gm3_pos_dot(norm, l_vec);
  if (dot < 0)
    dot = 0; // Light is behind the face

  // Calculate final intensity (Light Intensity * Angle * Distance Falloff
  // [optional])
  double final_intensity = scene->light.intensity * dot;

  // Apply intensity to the light's color
  out->color = gm_scale_color(scene->light.color, final_intensity);

  // 3. Perspective Projection
  // x_2d = (x_3d / z_3d) * focal_length
  // We also center it on the screen.
  for (int i = 0; i < 3; i++) {
    // Perspective divide
    // The constant (e.g., screen_w) acts as the focal length/field of view.
    out->vertices[i].x =
        (float)((vertices[i].x / vertices[i].z) * scene->viewport.x +
                (scene->viewport.x / 2.0));
    out->vertices[i].y =
        (float)((vertices[i].y / vertices[i].z) * scene->viewport.y +
                (scene->viewport.y / 2.0));
  }
  out->visible = 1;
  return 1;
}

int gm3_project(gm3Mesh *mesh, gm3Transform *transform, gm3Scene *scene,
                gm3Image *output) {
  output->n_vertices = mesh->n_vertices;
  output->n_triangles = mesh->n_faces;
  output->n_colors = mesh->n_faces;
  output->vertices = calloc(output->n_vertices, sizeof(gm3Pos));
  output->triangles = calloc(output->n_triangles * 3, sizeof(size_t));
  output->colors = calloc(output->n_colors, sizeof(gmColor));

  for (size_t i = 0; i < mesh->n_vertices; i++) {
    gm3Pos point = gm3_pos_rotate(mesh->vertices[i], transform->rotation);
    point = gm3_pos_translate(point, transform->position);
    point = gm3_pos_scale(point, transform->scale);

    // very simple projection x' = x / z, y' = y / z
    output->vertices[i] = gm3_pos_project_simple(point);
  }
  gm3TriangleImage *face;

  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3Pos vertices[3];
    for (size_t j = 0; j < 3; j++)
      vertices[j] = mesh->vertices[mesh->faces[i].vertices[j]];
    if (gm3_project_face(face, mesh->normals[mesh->faces[i].normal], vertices,
                         scene)) {
      // ama
      // //
    }
  }

  return 0;
}
