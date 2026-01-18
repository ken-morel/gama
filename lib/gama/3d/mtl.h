/**
 * @file mtl.h
 * @brief Defines structures for 3D materials and material libraries, and functions for loading MTL files.
 *
 * This file handles parsing and representing material data typically found
 * in .mtl files accompanying .obj models. It includes material properties
 * like colors, shininess, alpha, and references to texture maps.
 */

 #ifndef GM3_MTL_H_INCLUDED
 #define GM3_MTL_H_INCLUDED
#include "../color.h"
#include "../image.h"
#include "../str.h"
#include "../utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Represents a single 3D material with various rendering properties.
 *
 * This struct stores properties like diffuse color, specular color, shininess,
 * transparency, and references to different texture maps.
 */
typedef struct {
  char name[64];    /**< The name of the material. */
  gmColor diffuse;  /**< Diffuse color (Kd) of the material. */
  gmColor specular; /**< Specular color (Ks) of the material. */
  double shininess; /**< Shininess exponent (Ns), typically 0 to 1000. */
  double alpha;     /**< Transparency (d or Tr), 1.0 is opaque, 0.0 is fully transparent. */
  gmColor emissive; /**< Emissive color (Ke) of the material. */

  long tex_diffuse; /**< Index to the diffuse texture map in the material library's texture array (-1 if none). */
  long tex_specular; /**< Index to the specular texture map (-1 if none). */
  long tex_alpha;   /**< Index to the alpha (dissolve) texture map (-1 if none). */
  long tex_emissive; /**< Index to the emissive texture map (-1 if none). */
} gm3Material;

/**
 * @brief Represents a 3D texture, including its raw image data and file path.
 */
typedef struct {
  gmImageData data; /**< The raw CPU-side image data for the texture. */
  char path[256];   /**< The file path from which the texture was loaded. */
} gm3Texture;

/**
 * @brief Represents a material library, typically loaded from an .mtl file.
 *
 * This struct contains a collection of materials and the textures they reference.
 */
typedef struct {
  char name[256];        /**< The name of the material library (usually the MTL filename). */

  gm3Material *materials; /**< Array of materials defined in this library. */
  size_t n_materials;    /**< Number of materials in the array. */

  gm3Texture *textures;   /**< Array of textures referenced by materials in this library. */
  size_t n_textures;     /**< Number of textures in the array. */
} gm3MtlLib;

/**
 * @internal
 * @brief Skips leading whitespace characters in a string.
 * @param s The string to process.
 * @return A pointer to the first non-whitespace character in the string.
 */
static inline char *gm3u_skip_spaces(char *s) {
  while (*s && (*s == ' ' || *s == '\t'))
    s++;
  return s;
}

/**
 * @brief Adds a texture to the material library, loading it if not already present.
 *
 * This function checks if a texture with the given path already exists in the
 * `mtllib`. If so, it returns its index. Otherwise, it loads the image data,
 * adds it to the `mtllib`'s texture array, and returns the new index.
 *
 * @param mtllib A pointer to the `gm3MtlLib` to add the texture to.
 * @param path The file path of the texture to add.
 * @return The index of the texture in the `mtllib->textures` array on success, -1 on failure.
 */
long gm3_mtl_add_texture(gm3MtlLib *mtllib, const char *path) {
  for (size_t i = 0; i < mtllib->n_textures; i++)
    if (0 == strcmp(path, mtllib->textures[i].path))
      return i;
  gm3Texture *tex =
      realloc(mtllib->textures, sizeof(gm3Texture) * (mtllib->n_textures + 1));
  if (!tex)
    return -1;
  mtllib->textures = tex;
  tex = &mtllib->textures[mtllib->n_textures];
  mtllib->n_textures++;

  int ret = gm_image_data_load(&tex->data, path);
  if (ret < 0)
    return ret;
  memset(tex->path, 0, sizeof(tex->path));
  memcpy(tex->path, path, strlen(path));
  return mtllib->n_textures - 1;
}

/**
 * @brief Loads materials and textures from an OBJ-style .mtl file.
 *
 * This function parses the specified .mtl file, extracting material properties
 * and loading any referenced textures.
 *
 * @param mtl_lib A pointer to the `gm3MtlLib` struct to populate with loaded data.
 * @param path The file path to the .mtl file.
 * @param dir The base directory for resolving relative texture paths.
 * @return 0 on success, -1 on memory allocation failure, -2 if the file cannot be opened,
 *         or a negative value from `gm3_mtl_add_texture` on texture loading failure.
 */
int gm3_mtl_load(gm3MtlLib *mtl_lib, const char *path, const char *dir) {
  mtl_lib->name[0] = '\0';
  mtl_lib->materials = NULL;
  mtl_lib->n_materials = 0;

  FILE *f = fopen(path, "r");
  if (!f) {
    return -2;
  }
  gmu_get_filename_base(path, mtl_lib->name, sizeof(mtl_lib->name));
  char line[512];
  gm3Material *current = NULL;

  while (fgets(line, sizeof(line), f)) {
    char *p = gm3u_skip_spaces(line);
    if (*p == '\0' || *p == '#')
      continue;

    if (strncmp(p, "newmtl ", 7) == 0) {
      mtl_lib->n_materials++;
      mtl_lib->materials = realloc(mtl_lib->materials,
                                   sizeof(gm3Material) * mtl_lib->n_materials);
      current = &mtl_lib->materials[mtl_lib->n_materials - 1];

      // Initialize defaults
      memset(current, 0, sizeof(gm3Material));
      current->alpha = 1.0;
      current->diffuse = gm_rgb(200, 200, 200);
      sscanf(p + 7, "%63s", current->name);
    } else if (current) {
      if (strncmp(p, "Kd ", 3) == 0) {
        float r, g, b;
        sscanf(p + 3, "%f %f %f", &r, &g, &b);
        current->diffuse =
            gm_rgb((int)(r * 255), (int)(g * 255), (int)(b * 255));
      } else if (strncmp(p, "Ks ", 3) == 0) {
        float r, g, b;
        sscanf(p + 3, "%f %f %f", &r, &g, &b);
        current->specular =
            gm_rgb((int)(r * 255), (int)(g * 255), (int)(b * 255));
      } else if (strncmp(p, "Ns ", 3) == 0) {
        current->shininess = atof(p + 3);
      } else if (p[0] == 'd' && isspace(p[1])) {
        current->alpha = atof(p + 2);
      } else if (strncmp(p, "Tr ", 3) == 0) {
        current->alpha = 1.0 - atof(p + 3); // Tr is transparency
      } else if (strncmp(p, "map_Kd", 6) == 0) {
        char tex_path[256] = {0};
        gm3u_str_copy_eol(tex_path, gm3u_skip_spaces(p + 6), sizeof(tex_path));
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tex_path);
        long ret = gm3_mtl_add_texture(mtl_lib, full_path);
        if (ret < 0) {
          fclose(f);
          return (int)ret;
        }
        current->tex_diffuse = ret;
      } else if (strncmp(p, "map_Ks", 6) == 0) {
        char tex_path[256] = {0};
        gm3u_str_copy_eol(tex_path, gm3u_skip_spaces(p + 6), sizeof(tex_path));
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tex_path);
        long ret = gm3_mtl_add_texture(mtl_lib, full_path);
        if (ret < 0) {
          fclose(f);
          return (int)ret;
        }
        current->tex_specular = ret;
      } else if (strncmp(p, "map_Ke", 6) == 0) {
        char tex_path[256] = {0};
        gm3u_str_copy_eol(tex_path, gm3u_skip_spaces(p + 6), sizeof(tex_path));
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tex_path);
        long ret = gm3_mtl_add_texture(mtl_lib, full_path);
        if (ret < 0) {
          fclose(f);
          return (int)ret;
        }
        current->tex_emissive = ret;
      } else if (strncmp(p, "map_d", 5) == 0) {
        char tex_path[256] = {0};
        gm3u_str_copy_eol(tex_path, gm3u_skip_spaces(p + 5), sizeof(tex_path));
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tex_path);
        long ret = gm3_mtl_add_texture(mtl_lib, full_path);
        if (ret < 0) {
          fclose(f);
          return (int)ret;
        }
        current->tex_alpha = ret;
      }
    }
  }

  fclose(f);
  return 0;
}

/**
 * @brief Finds a material by name within a loaded MTL file.
 */
gm3Material *gm3_mtl_find_mat(gm3MtlLib *file, const char *name, int *index) {

  if (!file) {
    if (index)
      *index = -1;
    return NULL;
  }
  if (index)
    *index = -1;
  for (size_t i = 0; i < file->n_materials; i++) {
    if (strcmp(file->materials[i].name, name) == 0) {
      if (index)
        *index = i;
      return &file->materials[i];
    }
    }
  return NULL;
}

void gm3_mtl_free(gm3MtlLib *file) {
  if (file) {
    if (file->materials)
      free(file->materials);
    free(file);
  }
}
#endif // GM3_MTL_H_INCLUDED
