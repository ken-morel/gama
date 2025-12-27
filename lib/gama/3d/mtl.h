#pragma once

#include "../color.h"
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
  char path[256];
  gm3Material *materials;
  size_t n_materials;
} gm3MtlFile;

// Global/Context registry to prevent double-loading
typedef struct {
  gm3MtlFile *files;
  size_t n_files;
} gm3MtlLibrary;

// Helper to skip whitespace
static inline char *gm3_u_skip_spaces(char *s) {
  while (*s && (*s == ' ' || *s == '\t'))
    s++;
  return s;
}

/**
 * Loads a .mtl file and returns its pointer.
 */
int gm3_mtl_load(gm3MtlFile *mtl_file, const char *path) {
  mtl_file->path[0] = '\0';
  mtl_file->materials = NULL;
  mtl_file->n_materials = 0;

  FILE *f = fopen(path, "r");
  if (!f) {
    printf("MTL: Could not open file %s\n", path);
    return -1;
  }

  strncpy(mtl_file->path, path, sizeof(mtl_file->path) - 1);
  mtl_file->path[sizeof(mtl_file->path) - 1] = '\0';
  char line[512];
  gm3Material *current = NULL;

  while (fgets(line, sizeof(line), f)) {
    char *p = gm3_u_skip_spaces(line);
    if (*p == '\0' || *p == '#')
      continue;

    if (strncmp(p, "newmtl ", 7) == 0) {
      mtl_file->n_materials++;
      mtl_file->materials = realloc(
          mtl_file->materials, sizeof(gm3Material) * mtl_file->n_materials);
      current = &mtl_file->materials[mtl_file->n_materials - 1];

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
gm3Material *gm3_mtl_find(gm3MtlFile *file, const char *name) {
  if (!file)
    return NULL;
  for (size_t i = 0; i < file->n_materials; i++) {
    if (strcmp(file->materials[i].name, name) == 0) {
      return &file->materials[i];
    }
  }
  return NULL;
}

void gm3_mtl_free(gm3MtlFile *file) {
  if (file) {
    if (file->materials)
      free(file->materials);
    free(file);
  }
}
