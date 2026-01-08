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
#include <stddef.h>
#include <stdlib.h>

// --- Optimized Lighting ---

static inline gmColor gm3_calculate_lighting(gm3Pos norm, gm3Pos face_center,
                                             const gm3Material *mat,
                                             const gm3Scene *scene) {
  // 1. Vector Setup (Manually inlined for speed)

  // Light Vector (L): From Surface to Light
  double lx = scene->light.position.x - face_center.x;
  double ly = scene->light.position.y - face_center.y;
  double lz = scene->light.position.z - face_center.z;

  // Normalize L
  double l_len_sq = lx * lx + ly * ly + lz * lz;
  double l_inv_len = 1.0 / sqrt(l_len_sq);
  lx *= l_inv_len;
  ly *= l_inv_len;
  lz *= l_inv_len;

  // 2. Diffuse Component (N dot L)
  double dot_diff = norm.x * lx + norm.y * ly + norm.z * lz;
  double diffuse = (dot_diff > 0.0) ? dot_diff : 0.0;

  // 3. Specular Component (Blinn-Phong)
  double specular = 0.0;

  // Only calculate specular if the face is lit and material is shiny
  if (diffuse > 0.0 && mat && mat->shininess > 0.0) {
    // View Vector (V): From Surface to Camera (0,0,0)
    // So V = (0,0,0) - FaceCenter = -FaceCenter
    double vx = -face_center.x;
    double vy = -face_center.y;
    double vz = -face_center.z;

    // Normalize V
    double v_len_sq = vx * vx + vy * vy + vz * vz;
    double v_inv_len = 1.0 / sqrt(v_len_sq);
    vx *= v_inv_len;
    vy *= v_inv_len;
    vz *= v_inv_len;

    // Half Vector (H) = Normalize(L + V)
    double hx = lx + vx;
    double hy = ly + vy;
    double hz = lz + vz;

    double h_len_sq = hx * hx + hy * hy + hz * hz;

    // Avoid divide by zero if L and V are exactly opposite
    if (h_len_sq > 0.000001) {
      double h_inv_len = 1.0 / sqrt(h_len_sq);
      hx *= h_inv_len;
      hy *= h_inv_len;
      hz *= h_inv_len;

      // N dot H
      double dot_spec = norm.x * hx + norm.y * hy + norm.z * hz;
      if (dot_spec > 0.0) {
        // Expensive POW call, only done when strictly necessary
        specular = pow(dot_spec, mat->shininess);
      }
    }
  }

  // 4. Combine Light Intensities
  double intensity = scene->light.intensity;
  double ambient = scene->light.ambient;

  // Pre-calculate common factors
  // Diffuse affects the material color
  double light_factor = (diffuse * intensity) + ambient;
  // Specular is usually white (light color) added on top
  double spec_factor = specular * intensity;

  // 5. Final Color Mixing
  // Light Source Color (normalized 0-1)
  double lr = gm_red(scene->light.color) / 255.0;
  double lg = gm_green(scene->light.color) / 255.0;
  double lb = gm_blue(scene->light.color) / 255.0;

  int r, g, b, a;

  if (mat) {
    double mr = gm_red(mat->diffuse);
    double mg = gm_green(mat->diffuse);
    double mb = gm_blue(mat->diffuse);

    double msr = gm_red(mat->specular);
    double msg = gm_green(mat->specular);
    double msb = gm_blue(mat->specular);

    // Color = (MatDiffuse * LightFactor * LightColor) + (MatSpec * SpecFactor *
    // LightColor)
    r = (int)((mr * light_factor + msr * spec_factor) * lr);
    g = (int)((mg * light_factor + msg * spec_factor) * lg);
    b = (int)((mb * light_factor + msb * spec_factor) * lb);
    a = (int)(mat->alpha * 255);
  } else {
    // Default white material
    double k = light_factor + spec_factor; // Simple mix
    r = (int)(lr * 255.0 * k);
    g = (int)(lg * 255.0 * k);
    b = (int)(lb * 255.0 * k);
    a = 255;
  }

  // Clamping (Manual min/max is faster than fmax/fmin calls usually)
  if (r > 255)
    r = 255;
  if (r < 0)
    r = 0;
  if (g > 255)
    g = 255;
  if (g < 0)
    g = 0;
  if (b > 255)
    b = 255;
  if (b < 0)
    b = 0;

  return gm_rgba(r, g, b, a);
}

// --- Main Projection Function ---

int gm3_project(gm3Image *output, const gm3Mesh *mesh,
                const gm3Transform *transform, const gm3Scene *scene) {

  if (!mesh || mesh->n_vertices == 0)
    return 0;

  // 1. Ensure Capacity in Output Image
  if (!gm3_image_ensure_cap(output, output->n_vertices + mesh->n_vertices,
                            output->n_triangles + mesh->n_faces)) {
    return -1;
  }

  // 2. Ensure Scratch Buffer (World Coordinates) Capacity
  // This buffer is inside the image struct, safe for this specific image
  // instance
  if (mesh->n_vertices > output->_internal.cap_world) {
    size_t new_cap = mesh->n_vertices + 512;
    void *tmp =
        realloc(output->_internal.world_verts, new_cap * sizeof(gm3Pos));
    if (!tmp)
      return -1;
    output->_internal.world_verts = tmp;
    output->_internal.cap_world = new_cap;
  }

  // Access pointers for speed
  gm3Pos *scratch_world = output->_internal.world_verts;
  size_t start_v = output->n_vertices;
  size_t start_t = output->n_triangles;

  double focal = scene->camera.focal;
  double near_plane = scene->camera.near;
  double far_plane = scene->camera.far;

  // ========================================================
  // STAGE A: VERTEX PROCESSING (Linear O(V))
  // Transform Local -> World -> Screen
  // ========================================================
  for (size_t i = 0; i < mesh->n_vertices; i++) {
    // 1. Model -> World
    gm3Pos p = mesh->vertices[i];
    gm3_transform_pos(&p, transform);

    // Save World position for Lighting/Culling later
    scratch_world[i] = p;

    // 2. World -> Screen (Projected)
    if (p.z > 0.1) {
      double inv_z = 1.0 / p.z; // Optimization: Mult is faster than Div
      double s = focal * inv_z;

      output->vertices[start_v + i].x = p.x * s;
      output->vertices[start_v + i].y = p.y * s;
      // Z is not stored in vertex buffer, used later for depth buffer
    } else {
      // Behind camera: mark as garbage
      output->vertices[start_v + i].x = -99999.0;
      output->vertices[start_v + i].y = -99999.0;
    }
  }

  // Update total vertices in image
  output->n_vertices += mesh->n_vertices;

  // ========================================================
  // STAGE B: FACE PROCESSING (Linear O(F))
  // Connect vertices, Cull, Light
  // ========================================================
  size_t t_idx = start_t;

  for (size_t i = 0; i < mesh->n_faces; i++) {
    gm3MeshFace *face = &mesh->faces[i];

    // Get the World Positions we calculated in Stage A
    gm3Pos *w0 = &scratch_world[face->vertices[0]];
    gm3Pos *w1 = &scratch_world[face->vertices[1]];
    gm3Pos *w2 = &scratch_world[face->vertices[2]];

    // 1. Clipping / Safety Check
    if (w0->z < near_plane || w0->z > far_plane)
      continue;
    if (w1->z < 0.1 || w2->z < 0.1)
      continue; // Basic near clip

    // 2. Face Center & Backface Culling
    // Inline Center Calculation
    gm3Pos center = {(w0->x + w1->x + w2->x) * 0.333333,
                     (w0->y + w1->y + w2->y) * 0.333333,
                     (w0->z + w1->z + w2->z) * 0.333333};

    // Calculate Normal (Rotated)
    gm3Pos norm = face->normal;
    gm3_pos_rotate(&norm, &transform->rotation);

    // Backface Check: Dot(Normal, Vector_to_Cam)
    // Camera is at (0,0,0), so Vector_to_Cam is (0 - center).
    // Dot(N, -C) > 0  ==  Dot(N, C) < 0
    if (gm3_pos_dot(norm, center) >= 0)
      continue;

    // 3. Lighting
    gm3Material *mat = NULL;
    if (face->material_file >= 0 && face->material >= 0) {
      // Assuming bounds checks happen in mesh loader or are safe
      mat = &mesh->mtllibs[face->material_file].materials[face->material];
    }
    output->colors[t_idx] = gm3_calculate_lighting(norm, center, mat, scene);

    // 4. Triangle Assembly
    // Point to the global indices in the image vertex array
    output->triangles[t_idx * 3 + 0] = start_v + face->vertices[0];
    output->triangles[t_idx * 3 + 1] = start_v + face->vertices[1];
    output->triangles[t_idx * 3 + 2] = start_v + face->vertices[2];

    output->depths[t_idx] = center.z;
    t_idx++;
  }

  output->n_triangles = t_idx;
  return 0;
}
