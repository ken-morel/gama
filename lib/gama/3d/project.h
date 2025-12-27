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
  short unsigned ignore_backward_faces;
} gm3Project = {
    .ignore_backward_faces = 1,
};

/**
 * Transforms an entire mesh and projects it into a gm3Image.
 */
int gm3_project(gm3Mesh *mesh, gm3Transform *transform, gm3Scene *scene,
                gm3Image *output) {

  size_t start_vertex = output->n_vertices;
  size_t start_triangle = output->n_triangles;

  // 1. Safe Reallocation
  void *tmp_v = realloc(output->vertices,
                        (start_vertex + mesh->n_vertices) * sizeof(gm3Pos));
  void *tmp_t = realloc(output->triangles,
                        (start_triangle + mesh->n_faces) * 3 * sizeof(size_t));
  void *tmp_c = realloc(output->colors,
                        (start_triangle + mesh->n_faces) * sizeof(gmColor));
  void *tmp_d = realloc(output->depths,
                        (start_triangle + mesh->n_faces) * sizeof(double));

  if (!tmp_v || !tmp_t || !tmp_c || !tmp_d)
    return -1; // Allocation failure

  output->vertices = tmp_v;
  output->triangles = tmp_t;
  output->colors = tmp_c;
  output->depths = tmp_d;

  // Important: We update n_vertices now so the offset logic works
  output->n_vertices += mesh->n_vertices;

  // 2. World Space Transformation
  gm3Pos *world_verts = malloc(sizeof(gm3Pos) * mesh->n_vertices);
  for (size_t i = 0; i < mesh->n_vertices; i++) {
    gm3Pos p = mesh->vertices[i];
    p = gm3_pos_scale(p, transform->scale);
    p = gm3_pos_rotate(p, transform->rotation);
    p = gm3_pos_translate(p, transform->position);
    world_verts[i] = p;
  }

  // 3. Face Processing
  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3MeshFace *face = &mesh->faces[i];
    gm3Pos tri_verts[3] = {world_verts[face->vertices[0]],
                           world_verts[face->vertices[1]],
                           world_verts[face->vertices[2]]};

    gm3Pos world_norm =
        gm3_pos_rotate(mesh->normals[face->normal], transform->rotation);

    // Simple Backface Culling (Camera is at origin looking towards +Z)
    // If dot(normal, vector_to_camera) > 0, cull.
    // Simplified: if normal.z > 0, it faces away from camera.
    if (gm3Project.ignore_backward_faces && world_norm.z > 0)
      continue;

    gm3TriangleImage projected;
    if (gm3_project_face(&projected, world_norm, tri_verts, scene)) {
      size_t t_idx = output->n_triangles;

      // Copy projected 2D vertices into the global vertex buffer
      for (int j = 0; j < 3; j++) {
        size_t local_v_idx = face->vertices[j];
        size_t global_v_idx = local_v_idx + start_vertex;

        output->vertices[global_v_idx] = projected.vertices[j];
        output->triangles[t_idx * 3 + j] = global_v_idx; // FIXED: Added offset
      }

      output->depths[t_idx] = projected.depth;
      output->colors[t_idx] = projected.color;
      output->n_triangles++;
    }
  }

  free(world_verts);
  return 0;
}
