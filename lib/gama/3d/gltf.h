#pragma once

#include <stdio.h>
#include <string.h>

#include "mesh.h"
#include "position.h"

// Forward declarations
int gm3_gltf_load(gm3Mesh *mesh, const char *path);
int gmd_gltf_print(const char *path);

// -- IMPLEMENTATION --

#define CGLTF_IMPLEMENTATION
#include "../../cgltf.h"

static void gmd_gltf_print_node(cgltf_node *node, int level) {
  for (int i = 0; i < level; ++i)
    printf("  ");
  printf("Node: %s\n", node->name ? node->name : "(unnamed)");

  if (node->mesh) {
    for (int i = 0; i < level + 1; ++i)
      printf("  ");
    printf("Mesh: %s (%zu primitives)\n",
           node->mesh->name ? node->mesh->name : "(unnamed)",
           node->mesh->primitives_count);
  }

  for (size_t i = 0; i < node->children_count; ++i) {
    gmd_gltf_print_node(node->children[i], level + 1);
  }
}

int gmd_gltf_print(const char *path) {
  cgltf_options options = {0};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);
  if (result != cgltf_result_success) {
    printf("gmd_gltf_print: Failed to parse GLTF file: %s\n", path);
    return -1;
  }

  printf("--- GLTF Inspector: %s ---\n", path);
  printf("Scenes: %zu\n", data->scenes_count);
  if (data->scene) {
    printf("Default Scene: '%s' (%zu root nodes)\n",
           data->scene->name ? data->scene->name : "unnamed",
           data->scene->nodes_count);
    for (size_t i = 0; i < data->scene->nodes_count; ++i) {
      gmd_gltf_print_node(data->scene->nodes[i], 1);
    }
  }
  printf("Meshes: %zu\n", data->meshes_count);
  printf("Materials: %zu\n", data->materials_count);
  printf("Textures: %zu\n", data->textures_count);
  printf("Images: %zu\n", data->images_count);
  printf("---------------------------------\n");

  cgltf_free(data);
  return 0;
}

static void transform_pos(gm3Pos *dst, const gm3Pos *src,
                          const cgltf_float *m) {
  dst->x = src->x * m[0] + src->y * m[4] + src->z * m[8] + m[12];
  dst->y = src->x * m[1] + src->y * m[5] + src->z * m[9] + m[13];
  dst->z = src->x * m[2] + src->y * m[6] + src->z * m[10] + m[14];
}

static void transform_normal(gm3Pos *dst, const gm3Pos *src,
                             const cgltf_float *m) {
  // Apply the 3x3 rotation part of the matrix
  dst->x = src->x * m[0] + src->y * m[4] + src->z * m[8];
  dst->y = src->x * m[1] + src->y * m[5] + src->z * m[9];
  dst->z = src->x * m[2] + src->y * m[6] + src->z * m[10];
  gm3_pos_normalize(dst);
}

int gm3_gltf_load(gm3Mesh *mesh, const char *path) {
  memset(mesh, 0, sizeof(gm3Mesh));

  cgltf_options options = {0};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);
  if (result != cgltf_result_success)
    return -1;
  result = cgltf_load_buffers(&options, data, path);
  if (result != cgltf_result_success) {
    cgltf_free(data);
    return -1;
  }

  // Count totals first by iterating through nodes that will be rendered
  size_t total_vertices = 0;
  size_t total_indices = 0;
  size_t total_normals = 0;
  size_t total_texs = 0;
  for (size_t i = 0; i < data->nodes_count; ++i) {
    cgltf_node *node = &data->nodes[i];
    if (!node->mesh)
      continue;
    for (size_t j = 0; j < node->mesh->primitives_count; ++j) {
      cgltf_primitive *prim = &node->mesh->primitives[j];
      if (prim->type == cgltf_primitive_type_triangles && prim->indices) {
        total_indices += prim->indices->count;
        for (size_t k = 0; k < prim->attributes_count; k++) {
          cgltf_attribute *attr = &prim->attributes[k];
          if (attr->type == cgltf_attribute_type_position) {
            total_vertices += attr->data->count;
          } else if (attr->type == cgltf_attribute_type_normal) {
            total_normals += attr->data->count;
          } else if (attr->type == cgltf_attribute_type_texcoord) {
            total_texs += attr->data->count;
          }
        }
      }
    }
  }

  if (total_indices == 0) {
    cgltf_free(data);
    return 0;
  }

  // Allocate memory for our gm3Mesh
  mesh->n_vertices = total_vertices;
  mesh->vertices = calloc(mesh->n_vertices, sizeof(gm3Pos));
  mesh->n_normals = total_normals;
  mesh->normals = calloc(mesh->n_normals, sizeof(gm3Pos));
  mesh->n_texs = total_texs;
  mesh->texs = calloc(mesh->n_texs, sizeof(gm3Tex));
  mesh->n_faces = total_indices / 3;
  mesh->faces = calloc(mesh->n_faces, sizeof(gm3MeshFace));

  // Create a single material library for this glTF file
  mesh->n_mtllibs = 1;
  mesh->mtllibs = calloc(1, sizeof(gm3MtlLib));
  gm3MtlLib *mtllib = &mesh->mtllibs[0];
  strcpy(mtllib->name, "gltf_materials");

  // Pre-load all images into our material lib's textures
  mtllib->n_textures = data->images_count;
  mtllib->textures = calloc(mtllib->n_textures, sizeof(gm3Texture));
  for (size_t i = 0; i < data->images_count; ++i) {
    cgltf_image *gimg = &data->images[i];
    gm3Texture *dtex = &mtllib->textures[i];
    memset(dtex, 0, sizeof(gm3Texture));

    if (gimg->buffer_view) {
      snprintf(dtex->path, sizeof(dtex->path), "gltf_embedded_image_%zu", i);
      gm_image_data_load_from_memory(
          &dtex->data,
          (unsigned char *)gimg->buffer_view->buffer->data +
              gimg->buffer_view->offset,
          gimg->buffer_view->size);
    } else if (gimg->uri) {
      char dir[256] = ".";
      const char *slash = strrchr(path, '/');
      const char *backslash = strrchr(path, '\\');
      const char *last_separator = slash > backslash ? slash : backslash;
      if (last_separator) {
        size_t dir_len = last_separator - path;
        if (dir_len < sizeof(dir) - 1) {
          strncpy(dir, path, dir_len);
          dir[dir_len] = '\0';
        }
      }
      snprintf(dtex->path, sizeof(dtex->path), "%s/%s", dir, gimg->uri);
      gm_image_data_load(&dtex->data, dtex->path);
    }
  }

  // Process materials, linking to pre-loaded textures
  mtllib->n_materials = data->materials_count;
  mtllib->materials = calloc(mtllib->n_materials, sizeof(gm3Material));
  for (size_t i = 0; i < data->materials_count; ++i) {
    cgltf_material *gmat = &data->materials[i];
    gm3Material *dmat = &mtllib->materials[i];
    memset(dmat, 0, sizeof(gm3Material));
    dmat->tex_diffuse = -1;
    dmat->tex_emissive = -1;
    dmat->tex_specular = -1;
    dmat->tex_alpha = -1;

    if (gmat->name)
      snprintf(dmat->name, sizeof(dmat->name), "%s", gmat->name);
    else
      snprintf(dmat->name, sizeof(dmat->name), "material_%zu", i);

    dmat->alpha = gmat->alpha_mode == cgltf_alpha_mode_blend ? 0.5 : 1.0;
    if (gmat->has_pbr_metallic_roughness) {
      cgltf_pbr_metallic_roughness *pbr = &gmat->pbr_metallic_roughness;
      dmat->diffuse = gm_rgb(pbr->base_color_factor[0] * 255,
                             pbr->base_color_factor[1] * 255,
                             pbr->base_color_factor[2] * 255);
      dmat->alpha = pbr->base_color_factor[3];

      if (pbr->base_color_texture.texture &&
          pbr->base_color_texture.texture->image) {
        dmat->tex_diffuse =
            pbr->base_color_texture.texture->image - data->images;
      }
    }
    dmat->emissive =
        gm_rgb(gmat->emissive_factor[0] * 255, gmat->emissive_factor[1] * 255,
               gmat->emissive_factor[2] * 255);
    if (gmat->emissive_texture.texture &&
        gmat->emissive_texture.texture->image) {
      dmat->tex_emissive = gmat->emissive_texture.texture->image - data->images;
    }
  }

  // Process nodes and primitives
  size_t v_offset = 0, n_offset = 0, t_offset = 0, face_offset = 0;
  for (size_t i = 0; i < data->nodes_count; ++i) {
    cgltf_node *node = &data->nodes[i];
    if (!node->mesh)
      continue;

    cgltf_float matrix[16];
    cgltf_node_transform_world(node, matrix);

    for (size_t j = 0; j < node->mesh->primitives_count; ++j) {
      cgltf_primitive *prim = &node->mesh->primitives[j];
      if (prim->type != cgltf_primitive_type_triangles || !prim->indices)
        continue;

      cgltf_accessor *pos_accessor = NULL;
      cgltf_accessor *nrm_accessor = NULL;
      cgltf_accessor *uv_accessor = NULL;
      for (size_t k = 0; k < prim->attributes_count; k++) {
        if (prim->attributes[k].type == cgltf_attribute_type_position)
          pos_accessor = prim->attributes[k].data;
        else if (prim->attributes[k].type == cgltf_attribute_type_normal)
          nrm_accessor = prim->attributes[k].data;
        else if (prim->attributes[k].type == cgltf_attribute_type_texcoord &&
                 prim->attributes[k].index == 0)
          uv_accessor = prim->attributes[k].data;
      }
      if (!pos_accessor)
        continue;

      size_t primitive_v_count = pos_accessor->count;
      size_t primitive_n_count = nrm_accessor ? nrm_accessor->count : 0;
      size_t primitive_t_count = uv_accessor ? uv_accessor->count : 0;

      // Read and transform vertices
      for (size_t k = 0; k < primitive_v_count; ++k) {
        gm3Pos p_local = {0};
        cgltf_accessor_read_float(pos_accessor, k, (cgltf_float *)&p_local, 3);
        transform_pos(&mesh->vertices[v_offset + k], &p_local, matrix);
      }

      // Read and transform normals
      if (nrm_accessor) {
        for (size_t k = 0; k < primitive_n_count; ++k) {
          gm3Pos n_local = {0};
          cgltf_accessor_read_float(nrm_accessor, k, (cgltf_float *)&n_local,
                                    3);
          transform_normal(&mesh->normals[n_offset + k], &n_local, matrix);
        }
      }

      // Read UVs
      if (uv_accessor) {
        for (size_t k = 0; k < primitive_t_count; ++k) {
          cgltf_accessor_read_float(
              uv_accessor, k, (cgltf_float *)&mesh->texs[t_offset + k], 2);
        }
      }

      // Create faces
      for (size_t k = 0; k < prim->indices->count; k += 3) {
        gm3MeshFace *face = &mesh->faces[face_offset++];
        face->vertices[0] =
            v_offset + cgltf_accessor_read_index(prim->indices, k + 0);
        face->vertices[1] =
            v_offset + cgltf_accessor_read_index(prim->indices, k + 1);
        face->vertices[2] =
            v_offset + cgltf_accessor_read_index(prim->indices, k + 2);

        if (uv_accessor) {
          face->uvs[0] =
              t_offset + cgltf_accessor_read_index(prim->indices, k + 0);
          face->uvs[1] =
              t_offset + cgltf_accessor_read_index(prim->indices, k + 1);
          face->uvs[2] =
              t_offset + cgltf_accessor_read_index(prim->indices, k + 2);
        }

        face->material_file = 0; // Only one material file
        face->material =
            prim->material ? (int)(prim->material - data->materials) : -1;

        // Calculate face normal from vertices (geometric normal)
        gm3Pos p0 = mesh->vertices[face->vertices[0]];
        gm3Pos p1 = mesh->vertices[face->vertices[1]];
        gm3Pos p2 = mesh->vertices[face->vertices[2]];
        gm3Pos e1 = p1;
        gm3_pos_substract(&e1, &p0);
        gm3Pos e2 = p2;
        gm3_pos_substract(&e2, &p0);
        face->normal = gm3_pos_cross(e1, e2);
        gm3_pos_normalize(&face->normal);
      }
      v_offset += primitive_v_count;
      n_offset += primitive_n_count;
      t_offset += primitive_t_count;
    }
  }

  gm3_mesh_center(mesh);
  cgltf_free(data);
  return 0;
}
