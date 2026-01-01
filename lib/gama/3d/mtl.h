#pragma once

#include "../color.h"
#include "../str.h"
#include "../utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[64];
  gmColor diffuse;  // Kd
  gmColor specular; // Ks
  double shininess; // Ns (0 to 1000)
  double alpha;     // d or Tr (1.0 is opaque)
  gmColor emissive; // Ke
                    // Add texture path here later if needed: char map_kd[256];
} gm3Material;

typedef struct {
  char name[256];

  gm3Material *materials;
  size_t n_materials;
} gm3MtlLib;

// Helper to skip whitespace
static inline char *gm3u_skip_spaces(char *s) {
  while (*s && (*s == ' ' || *s == '\t'))
    s++;
  return s;
}

/**
 * Loads a .mtl file.
 */
int gm3_mtl_load(gm3MtlLib *mtl_lib, const char *path) {
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
      }
    }
  }

  fclose(f);
  return 0;
}

/**
 * Finds a material by name within a loaded MTL file.
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

int gmg_material(gmStr *str, gm3Material mat) {

  char buffer[1024] = {0};

  sprintf(buffer, "(gm3Material){\n");
  gm_str_append(str, buffer);

  sprintf(buffer, "    .name = \"%s\",\n", mat.name);
  gm_str_append(str, buffer);

  // Diffuse
  gm_str_append(str, "    .diffuse = ");
  gmg_color(str, mat.diffuse);
  gm_str_append(str, ",\n");

  // Specular
  gm_str_append(str, "    .specular = ");
  gmg_color(str, mat.specular);
  gm_str_append(str, ",\n");

  // Emissive
  gm_str_append(str, "    .emissive = ");
  gmg_color(str, mat.emissive);
  gm_str_append(str, ",\n");

  sprintf(buffer, "    .shininess = %.4f,\n", mat.shininess);
  gm_str_append(str, buffer);

  sprintf(buffer, "    .alpha = %.4f\n", mat.alpha);
  gm_str_append(str, buffer);

  gm_str_append(str, "  }");
  return 0;
}

int gmg_mtllib(gmStr *str, gm3MtlLib lib) {
  char buffer[1024] = {0};

  sprintf(buffer, "(gm3MtlLib){\n");
  gm_str_append(str, buffer);

  sprintf(buffer, "  .name = \"%s\",\n", lib.name);
  gm_str_append(str, buffer);

  sprintf(buffer, "  .n_materials = %zu,\n", lib.n_materials);
  gm_str_append(str, buffer);

  if (lib.n_materials > 0) {
    gm_str_append(str, "  .materials = (gm3Material[]){\n");

    for (size_t i = 0; i < lib.n_materials; i++) {

      gm_str_append(str, "    ");
      gmg_material(str, lib.materials[i]);

      if (i < lib.n_materials - 1) {
        gm_str_append(str, ",\n");
      } else {
        gm_str_append(str, "\n");
      }
    }
    gm_str_append(str, "  }\n");
  } else {
    gm_str_append(str, "  .materials = NULL\n");
  }

  gm_str_append(str, "}");
  return 0;
}
