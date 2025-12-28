#pragma once

#include "../color.h"
#include "../position.h"
#include "image.h"
#include "light.h"
#include "mesh.h"
#include "mtl.h"
#include "obj.h"
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
  // View vector: from face to camera (Camera is at origin)
  gm3Pos v_vec = scene->camera_pos;
  gm3_pos_substract(&v_vec, &face_center);
  gm3_pos_normalize(&v_vec);
  // Halfway vector for Specular
  gm3Pos h_vec = l_vec;
  gm3_pos_add(&l_vec, &v_vec);
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
  double amb = scene->ambient;

  // Blend Light Color + Material Diffuse + Material Specular
  // Final = MatDiffuse * (DiffuseFactor * LightInt + Ambient) + (MatSpecular *
  // SpecularFactor * LightInt)
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
 * Projects a single face, calculates lighting, and checks visibility.
 */
int gm3_project_face(gm3TriangleImage *out, gm3Pos norm, gm3Pos *vertices,
                     gm3Material *mat, gm3Scene *scene) {

  gm3Pos face_center;
  gm3_pos_centerN(&face_center, vertices, 3);

  gm3Pos position_from_camera = face_center;
  gm3_pos_substract(&position_from_camera, &scene->camera_pos);
  double distance_from_camera = gm3_pos_magnitude(&position_from_camera);

  if (distance_from_camera <= scene->near || distance_from_camera >= scene->far)
    return 0;

  double normal_dot_position = gm3_pos_dot(&norm, &position_from_camera);
  if (normal_dot_position >= 0)
    return 0; // < 0 -> face points backwards , ==0 -> face -|

  out->color = gm3_calculate_lighting(norm, face_center, mat, scene);

  // 3. Perspective Projection
  out->depth = 0;
  for (int i = 0; i < 3; i++) {
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
} gm3Project = {};

/**
 * Transforms an entire mesh and projects it into a gm3Image.
 */
int gm3_project(gm3Mesh *mesh, gm3ObjFile *obj, gm3Transform *transform,
                gm3Scene *scene, gm3Image *output) {

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

    // Retrieve the material using the encoded index (file_idx << 16 | mat_idx)
    gm3Material *mat = NULL;
    size_t f_idx = face->material_idx >> 16;
    size_t m_idx = face->material_idx & 0xFFFF;

    if (obj && f_idx < obj->n_mtl_files &&
        m_idx < obj->mtl_files[f_idx]->n_materials) {
      mat = &obj->mtl_files[f_idx]->materials[m_idx];
    } // no fallback material
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
