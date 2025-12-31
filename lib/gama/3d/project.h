#pragma once

#include "../color.h"
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
  gm3Pos v_vec = {0, 0, 0};
  gm3_pos_substract(&v_vec, &face_center);
  gm3_pos_normalize(&v_vec);

  // Halfway vector for Specular
  gm3Pos h_vec = l_vec;
  gm3_pos_add(&h_vec, &v_vec);
  gm3_pos_normalize(&h_vec);

  // 2. Diffuse Component (Lambert)
  double dot_l = gm3_pos_dot(norm, l_vec);
  double diffuse_factor = (dot_l < 0) ? 0 : dot_l;

  // 3. Specular Component (Shiny Highlight)
  double specular_factor = 0;
  if (diffuse_factor > 0 && (!mat || mat->shininess > 0)) {
    double dot_h = gm3_pos_dot(norm, h_vec);
    if (dot_h > 0) {
      if (mat)
        specular_factor = exp(mat->shininess * (dot_h - 1.0));
      else
        specular_factor = 0;
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
 * Returns 1 if visible, 0 if culled.
 */
int gm3_project_face(gm3TriangleImage *out, gm3Pos norm, gm3Pos *vertices,
                     gm3Material *mat, gm3Scene *scene) {

  for (size_t i = 0; i < 3; i++) {
    if (vertices[i].z < 0)
      return 0;
  }
  // 1. Calculate Face Center for Lighting and Culling
  gm3Pos face_center = gm3_pos_centerN(vertices, 3);

  double distance = gm3_pos_magnitude(face_center);
  if (distance <= scene->camera.near || distance >= scene->camera.far)
    return 0;

  // 2. Correct Back-face Culling
  // Vector from FACE to CAMERA
  gm3Pos to_cam = {0, 0, 0}; // camera always at 0, 0, 0
  gm3_pos_substract(&to_cam, &face_center);

  // A face is visible only if the angle between the normal and the vector
  // to the camera is less than 90 degrees (Dot product > 0).
  if (gm3_pos_dot(norm, to_cam) <= 0)
    return 0;

  // 3. Lighting
  out->color = gm3_calculate_lighting(norm, face_center, mat, scene);

  // 4. Projection
  out->depth = distance;

  for (int i = 0; i < 3; i++) {
    // A. World Space -> Camera Space
    double x_cam = vertices[i].x;
    double y_cam = vertices[i].y;
    double z_cam = vertices[i].z;

    // C. Near Plane Clipping

    // D. Perspective Projection (NDC)
    // Applying the focal length here as a scale factor
    out->vertices[i].x = scene->camera.focal * x_cam / z_cam;
    out->vertices[i].y = scene->camera.focal * y_cam / z_cam;
  }
  return 1;
}

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
    return -1;

  output->vertices = tmp_v;
  output->triangles = tmp_t;
  output->colors = tmp_c;
  output->depths = tmp_d;
  output->n_vertices += mesh->n_vertices;

  // 2. World Space Transformation
  gm3Pos *world_verts = malloc(sizeof(gm3Pos) * mesh->n_vertices);
  if (!world_verts)
    return -1;

  for (size_t i = 0; i < mesh->n_vertices; i++) {
    gm3Pos p = mesh->vertices[i];
    gm3_transform_pos(&p, transform);
    world_verts[i] = p;
  }

  // 3. Face Processing
  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3MeshFace *face = &mesh->faces[i];
    gm3Pos tri_verts[3] = {world_verts[face->vertices[0]],
                           world_verts[face->vertices[1]],
                           world_verts[face->vertices[2]]};

    gm3Pos world_norm = face->normal;
    gm3_pos_rotate(&world_norm, &transform->rotation);

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
