#pragma once

#include "../color.h"

#include "../position.h"
#include "image.h"
#include "light.h"
#include "mesh.h"
#include "position.h"
#include "scene.h"
#include "transform.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * Projects a single face, calculates lighting, and checks visibility.
 * Note: Vertices and Normal must already be transformed into World/Camera
 * space.
 */
int gm3_project_face(gm3TriangleImage *out, gm3Pos norm, gm3Pos *vertices,
                     gm3Scene *scene) {

  // 1. Clipping & Visibility Check
  for (size_t i = 0; i < 3; i++) {
    // Basic Z-clipping against the scene's near/far planes
    if (vertices[i].z <= scene->near || vertices[i].z > scene->far) {
      return 0;
    }
  }

  // 2. Lighting Calculation (Lambertian Diffuse)
  gm3Pos face_center = gm3_pos_center3(vertices[0], vertices[1], vertices[2]);

  // Vector from face to light position
  gm3Pos l_vec = gm3_pos_minus(scene->light.position, face_center);
  l_vec = gm3_pos_normalize(l_vec);

  // Dot product: Normal · LightVector (cos of angle)
  double dot = gm3_pos_dot(norm, l_vec);
  if (dot < 0)
    dot = 0;

  // Combine Light Intensity and Angle
  double final_intensity = scene->light.intensity * dot;

  // Apply intensity to the light's color (ensure gm_scale_color is defined in
  // color.h)
  out->color = gm_scale_color(scene->light.color, final_intensity);

  // 3. Perspective Projection (3D -> 2D)
  out->depth = 0;
  for (int i = 0; i < 3; i++) {
    // Standard perspective: x' = (x/z) * focal_length + offset
    out->vertices[i].x =
        (float)((vertices[i].x / vertices[i].z) * scene->viewport.x +
                (scene->viewport.x / 2.0));
    out->vertices[i].y =
        (float)((vertices[i].y / vertices[i].z) * scene->viewport.y +
                (scene->viewport.y / 2.0));
    out->depth += vertices[i].z / 3.0;
  }

  return 1;
}

struct {
  int ignore_black;
} gm3Project = {
    .ignore_black = 0,
};

/**
 * Transforms an entire mesh and projects it into a gm3Image.
 */
int gm3_project(gm3Mesh *mesh, gm3Transform *transform, gm3Scene *scene,
                gm3Image *output) {

  // Prepare output structure
  // n_triangles might decrease if some are culled, but we allocate for the max
  output->n_vertices = mesh->n_vertices;
  output->vertices = calloc(output->n_vertices, sizeof(gm3Pos));
  output->triangles = calloc(mesh->n_faces * 3, sizeof(size_t));
  output->colors = calloc(mesh->n_faces, sizeof(gmColor));
  output->depths = calloc(mesh->n_faces, sizeof(double));
  output->n_colors = mesh->n_faces;
  output->n_triangles = 0;

  // 1. Transform all vertices into World Space first
  // We store them in a temporary buffer to avoid modifying the original mesh
  gm3Pos *world_verts = malloc(sizeof(gm3Pos) * mesh->n_vertices);
  for (size_t i = 0; i < mesh->n_vertices; i++) {
    gm3Pos p = mesh->vertices[i];
    p = gm3_pos_scale(p, transform->scale);
    p = gm3_pos_rotate(p, transform->rotation);
    p = gm3_pos_translate(p, transform->position);
    world_verts[i] = p;
  }

  // 2. Transform the normals (only rotation affects direction)
  // We simulate face normals by transforming the stored normal index
  // (Assuming mesh->normals contains the unique normal vectors)
  // In a real engine, you'd calculate the transformed normal per face or
  // transform the normal array.

  // 3. Process Faces
  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3MeshFace *face = &mesh->faces[i];
    gm3Pos tri_verts[3] = {world_verts[face->vertices[0]],
                           world_verts[face->vertices[1]],
                           world_verts[face->vertices[2]]};

    // Transform the normal for this face
    gm3Pos world_norm =
        gm3_pos_rotate(mesh->normals[face->normal], transform->rotation);

    if (world_norm.z > 0)
      continue;

    gm3TriangleImage projected;
    if (gm3_project_face(&projected, world_norm, tri_verts, scene)) {
      // Add to output image
      size_t t_idx = output->n_triangles;

      // Store the projected 2D coordinates back into the output vertex pool
      // (Note: This simple version overwrites shared vertices. In complex
      // engines, unique vertices per triangle are often preferred for flat
      // shading).
      for (int j = 0; j < 3; j++) {
        size_t v_idx = face->vertices[j];
        output->vertices[v_idx] = projected.vertices[j];
        output->triangles[t_idx * 3 + j] = v_idx;
      }
      output->depths[t_idx] = projected.depth;
      output->colors[t_idx] = projected.color;
      output->n_triangles++;
    }
  }

  free(world_verts);
  return 0;
}
