#pragma once

#include "../color.h"
#include "../debug.h"
#include "../position.h"
#include "image.h"
#include "light.h"
#include "mesh.h"
#include "mtl.h"
#include "position.h"
#include "scene.h"
#include "transform.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * Calculates the final color of a face using the Blinn-Phong model.
 */
static inline gmColor gm3_calculate_lighting(gm3Pos norm, gm3Pos face_center,
                                             gm3Material *mat,
                                             gm3Scene *scene) {
  // 1. Vector Setup

  // Light vector: from face to light
  gm3Pos l_vec = scene->light.position;
  gm3_pos_substract(&l_vec, &face_center);
  gm3_pos_normalize(&l_vec);

  // View vector: from face to camera
  // NOTE: Updated to use scene->camera.position
  gm3Pos v_vec = scene->camera.position;
  gm3_pos_substract(&v_vec, &face_center);
  gm3_pos_normalize(&v_vec);

  // Halfway vector for Specular
  gm3Pos h_vec = l_vec;
  gm3_pos_add(&h_vec, &v_vec);
  gm3_pos_normalize(&h_vec);

  // 2. Diffuse Component (Lambert)
  double dot_l = gm3_pos_dot(&norm, &l_vec);
  double diffuse_factor = (dot_l < 0) ? 0 : dot_l;

  // 3. Specular Component (Shiny Highlight)
  double specular_factor = 0;
  if (diffuse_factor > 0 && (!mat || mat->shininess > 0)) {
    double dot_h = gm3_pos_dot(&norm, &h_vec);
    if (dot_h > 0) {
      if (mat)
        specular_factor = exp(mat->shininess * (dot_h - 1.0));
      else
        specular_factor = dot_h;
    }
  }

  // 4. Intensity Application
  double intensity = scene->light.intensity;
  double amb = scene->light.ambient;

  int r, g, b, a;
  if (mat) {
    r = (int)(((gm_red(mat->diffuse) * (diffuse_factor * intensity + amb)) +
               (gm_red(mat->specular) * specular_factor * intensity)) *
              (gm_red(scene->light.color) / 255.0));
    g = (int)(((gm_green(mat->diffuse) * (diffuse_factor * intensity + amb)) +
               (gm_green(mat->specular) * specular_factor * intensity)) *
              (gm_green(scene->light.color) / 255.0));
    b = (int)(((gm_blue(mat->diffuse) * (diffuse_factor * intensity + amb)) +
               (gm_blue(mat->specular) * specular_factor * intensity)) *
              (gm_blue(scene->light.color) / 255.0));
    a = (int)(mat->alpha * 255);
  } else {
    double k = (specular_factor + diffuse_factor) * intensity + amb;
    r = (int)(gm_red(scene->light.color) * k);
    g = (int)(gm_green(scene->light.color) * k);
    b = (int)(gm_blue(scene->light.color) * k);
    a = 255;
  }
  return gm_rgba(r, g, b, a);
}

/**
 * Projects a single face to Normalized Device Coordinates (NDC).
 * * Input: World Space Vertices
 * Output: NDC Vertices (-1 to 1 range, assuming 90 deg FOV)
 */
int gm3_project_face(gm3TriangleImage *out, gm3Pos norm, gm3Pos *vertices,
                     gm3Material *mat, gm3Scene *scene) {

  printf("face: ");
  gmdn(pos3, norm);
  // 1. Calculate Face Center for Lighting
  gm3Pos face_center;
  gm3_pos_centerN(&face_center, vertices, 3);

  // 2. Back-face Culling
  gm3Pos view_dir = face_center;
  gm3_pos_substract(&view_dir, &scene->camera.position);

  // Normalize for dot product
  gm3Pos view_dir_norm = view_dir;
  gm3_pos_normalize(&view_dir_norm);

  // If face normal and view vector point same way, cull it.
  if (gm3_pos_dot(&norm, &view_dir_norm) >= 0)
    return 0;

  // 3. Lighting
  out->color = gm3_calculate_lighting(norm, face_center, mat, scene);

  // 4. Projection
  out->depth = 0;
  double inv_3 = 1.0 / 3.0;

  for (int i = 0; i < 3; i++) {
    // A. World Space -> Camera Space
    double x_cam = vertices[i].x - scene->camera.position.x;
    double y_cam = vertices[i].y - scene->camera.position.y;
    double z_cam = vertices[i].z - scene->camera.position.z;

    // B. Calculate Distance for Depth Buffer (Sorting)
    double dist = sqrt(x_cam * x_cam + y_cam * y_cam + z_cam * z_cam);
    out->depth += dist * inv_3;

    // C. Near Plane Clipping
    // Prevent divide by zero or negative Z projection
    if (z_cam <= 0.1)
      z_cam = 0.1;

    // D. Perspective Projection (NDC)
    // Formula: coord = cam / z
    // This implicitly assumes FOV of 90 degrees and Aspect Ratio 1:1
    out->vertices[i].x = (float)(x_cam / z_cam);
    out->vertices[i].y = (float)(y_cam / z_cam);
  }

  return 1;
}

struct {
} gm3Project = {};

/**
 * Transforms an entire mesh and projects it into a gm3Image.
 */
int gm3_project(gm3Mesh *mesh, gm3Transform *transform, gm3Scene *scene,
                gm3Image *output) {

  size_t start_vertex = output->n_vertices;
  size_t start_triangle = output->n_triangles;

  // 1. Safe Reallocation
  // If realloc fails, it returns NULL but does NOT free the original memory.
  // We use temporary pointers to ensure we don't corrupt the struct on failure.
  void *tmp_v = realloc(output->vertices,
                        (start_vertex + mesh->n_vertices) * sizeof(gm3Pos));
  void *tmp_t = realloc(output->triangles,
                        (start_triangle + mesh->n_faces) * 3 * sizeof(size_t));
  void *tmp_c = realloc(output->colors,
                        (start_triangle + mesh->n_faces) * sizeof(gmColor));
  void *tmp_d = realloc(output->depths,
                        (start_triangle + mesh->n_faces) * sizeof(double));

  // If ANY allocation fails, we cannot proceed.
  if (!tmp_v || !tmp_t || !tmp_c || !tmp_d) {
    // Note: In a production app, you might want to rollback the successful
    // reallocs or handle this more gracefully, but returning -1 here prevents a
    // crash.
    return -1;
  }

  // Commit changes only after all allocations succeed
  output->vertices = tmp_v;
  output->triangles = tmp_t;
  output->colors = tmp_c;
  output->depths = tmp_d;

  output->n_vertices += mesh->n_vertices;

  // 2. World Space Transformation
  gm3Pos *world_verts = malloc(sizeof(gm3Pos) * mesh->n_vertices);
  if (!world_verts) {
    return -1; // Fail if we can't allocate temp buffer
  }

  for (size_t i = 0; i < mesh->n_vertices; i++) {
    gm3Pos p = mesh->vertices[i];
    // gmdn(pos3, p);
    gm3_transform_pos(&p, transform);
    world_verts[i] = p;
    // gmdn(pos3, world_verts[i]);
  }

  // 3. Face Processing
  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3MeshFace *face = &mesh->faces[i];

    // Construct temp triangle from transformed vertices
    gm3Pos tri_verts[3] = {world_verts[face->vertices[0]],
                           world_verts[face->vertices[1]],
                           world_verts[face->vertices[2]]};

    gm3Pos world_norm = face->normal;
    gm3_pos_rotate(&world_norm, &transform->rotation);

    // Retrieve material
    gm3Material *mat = NULL;
    int f_idx = face->material_file;
    int m_idx = face->material;

    if (f_idx >= 0 && m_idx >= 0 && f_idx < mesh->n_mtllibs &&
        m_idx < mesh->mtllibs[f_idx].n_materials) {
      mat = &mesh->mtllibs[f_idx].materials[m_idx];
    }

    gm3TriangleImage projected;
    if (gm3_project_face(&projected, world_norm, tri_verts, mat, scene)) {
      size_t t_idx = output->n_triangles;

      for (int j = 0; j < 3; j++) {
        size_t global_v_idx = face->vertices[j] + start_vertex;
        output->vertices[global_v_idx] = projected.vertices[j];
        output->triangles[t_idx * 3 + j] = global_v_idx;
      }

      output->depths[t_idx] = projected.depth;
      output->colors[t_idx] = projected.color;
      output->n_triangles++;
    }
  }

  free(world_verts);
  return 0;
}
