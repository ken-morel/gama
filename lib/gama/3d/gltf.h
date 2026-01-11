#pragma once

#include <stdio.h>
#include <string.h>

// -- UTILS --
// Simple dynamic array for vertices, normals, etc.
#define GM_DYN_ARRAY_INIT_CAP 256
#define GM_DYN_ARRAY_APPEND(arr, count, capacity, item)                        \
  do {                                                                         \
    if ((count) >= (capacity)) {                                               \
      (capacity) = (capacity) == 0 ? GM_DYN_ARRAY_INIT_CAP : (capacity)*2;     \
      (arr) = realloc((arr), (capacity) * sizeof(*(arr)));                      \
    }                                                                          \
    (arr)[(count)++] = (item);                                                 \
  } while (0)

// -- HEADERS --
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
  if (cgltf_parse_file(&options, path, &data) != cgltf_result_success)
    return -1;
  if (cgltf_load_buffers(&options, data, path) != cgltf_result_success) {
    cgltf_free(data);
    return -1;
  }

  // Use dynamic arrays to avoid miscalculating buffer sizes
  size_t vertices_cap = 0, normals_cap = 0, texs_cap = 0, faces_cap = 0;

  // Create a single material library for this glTF file
  mesh->n_mtllibs = 1;
  mesh->mtllibs = calloc(1, sizeof(gm3MtlLib));
  gm3MtlLib *mtllib = &mesh->mtllibs[0];
  strcpy(mtllib->name, "gltf_materials");

  // Pre-load all images into our material lib's textures
  mtllib->n_textures = data->images_count;
  if (data->images_count > 0) {
    mtllib->textures = calloc(mtllib->n_textures, sizeof(gm3Texture));
    for (size_t i = 0; i < data->images_count; ++i) {
      cgltf_image *gimg = &data->images[i];
      gm3Texture *dtex = &mtllib->textures[i];
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
        if (slash) {
          size_t dir_len = slash - path;
          if (dir_len < sizeof(dir) - 1)
            strncpy(dir, path, dir_len);
        }
        snprintf(dtex->path, sizeof(dtex->path), "%s/%s", dir, gimg->uri);
        gm_image_data_load(&dtex->data, dtex->path);
      }
    }
  }

  // Process materials, linking to pre-loaded textures
  mtllib->n_materials = data->materials_count;
  if (data->materials_count > 0) {
    mtllib->materials = calloc(mtllib->n_materials, sizeof(gm3Material));
    for (size_t i = 0; i < data->materials_count; ++i) {
      cgltf_material *gmat = &data->materials[i];
      gm3Material *dmat = &mtllib->materials[i];
      *dmat = (gm3Material){.tex_diffuse = -1,
                            .tex_emissive = -1,
                            .tex_specular = -1,
                            .tex_alpha = -1};
      snprintf(dmat->name, sizeof(dmat->name), "%s",
               gmat->name ? gmat->name : "default");

      if (gmat->has_pbr_metallic_roughness) {
        cgltf_pbr_metallic_roughness *pbr = &gmat->pbr_metallic_roughness;
        dmat->diffuse =
            gm_rgb(pbr->base_color_factor[0] * 255,
                   pbr->base_color_factor[1] * 255,
                   pbr->base_color_factor[2] * 255);
        dmat->alpha = pbr->base_color_factor[3];

        if (pbr->base_color_texture.texture &&
            pbr->base_color_texture.texture->image) {
          dmat->tex_diffuse =
              pbr->base_color_texture.texture->image - data->images;
        }
      }
    }
  }

  // Process nodes and primitives
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

      size_t v_base_idx = mesh->n_vertices;
      size_t t_base_idx = mesh->n_texs;

      cgltf_accessor *pos_acc = NULL, *nrm_acc = NULL, *uv_acc = NULL;
      for (size_t k = 0; k < prim->attributes_count; k++) {
        if (prim->attributes[k].type == cgltf_attribute_type_position)
          pos_acc = prim->attributes[k].data;
        else if (prim->attributes[k].type == cgltf_attribute_type_normal)
          nrm_acc = prim->attributes[k].data;
        else if (prim->attributes[k].type == cgltf_attribute_type_texcoord)
          uv_acc = prim->attributes[k].data;
      }
      if (!pos_acc)
        continue;

      for (size_t k = 0; k < pos_acc->count; k++) {
        float p_float[3], n_float[3], t_float[2];
        gm3Pos p, n;
        gm3Tex t;

        cgltf_accessor_read_float(pos_acc, k, p_float, 3);
        p.x = p_float[0];
        p.y = p_float[1];
        p.z = p_float[2];
        transform_pos(&p, &p, matrix);
        GM_DYN_ARRAY_APPEND(mesh->vertices, mesh->n_vertices, vertices_cap, p);

        if (nrm_acc) {
          cgltf_accessor_read_float(nrm_acc, k, n_float, 3);
          n.x = n_float[0];
          n.y = n_float[1];
          n.z = n_float[2];
          transform_normal(&n, &n, matrix);
          GM_DYN_ARRAY_APPEND(mesh->normals, mesh->n_normals, normals_cap, n);
        }
        if (uv_acc) {
          cgltf_accessor_read_float(uv_acc, k, t_float, 2);
          t.u = t_float[0];
          t.v = t_float[1];
          GM_DYN_ARRAY_APPEND(mesh->texs, mesh->n_texs, texs_cap, t);
        }
      }

      for (size_t k = 0; k < prim->indices->count; k += 3) {
        gm3MeshFace face = {0};
        face.vertices[0] =
            v_base_idx + cgltf_accessor_read_index(prim->indices, k + 0);
        face.vertices[1] =
            v_base_idx + cgltf_accessor_read_index(prim->indices, k + 1);
        face.vertices[2] =
            v_base_idx + cgltf_accessor_read_index(prim->indices, k + 2);
        if (uv_acc) {
          face.uvs[0] =
              t_base_idx + cgltf_accessor_read_index(prim->indices, k + 0);
          face.uvs[1] =
              t_base_idx + cgltf_accessor_read_index(prim->indices, k + 1);
          face.uvs[2] =
              t_base_idx + cgltf_accessor_read_index(prim->indices, k + 2);
        }
        face.material =
            prim->material ? (int)(prim->material - data->materials) : -1;
        face.material_file = 0;

        gm3Pos p0 = mesh->vertices[face.vertices[0]];
        gm3Pos p1 = mesh->vertices[face.vertices[1]];
        gm3Pos p2 = mesh->vertices[face.vertices[2]];
        gm3Pos e1 = p1;
        gm3_pos_substract(&e1, &p0);
        gm3Pos e2 = p2;
        gm3_pos_substract(&e2, &p0);
        face.normal = gm3_pos_cross(e1, e2);
        gm3_pos_normalize(&face.normal);
        GM_DYN_ARRAY_APPEND(mesh->faces, mesh->n_faces, faces_cap, face);
      }
    }
  }

  gm3_mesh_center(mesh);
  cgltf_free(data);
  return 0;
}
