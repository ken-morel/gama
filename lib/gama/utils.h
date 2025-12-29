#pragma once

#include "_malloc.h"

#include <stdio.h>
#include <string.h>

void gmu_get_filename_stem(const char *path, char *out_stem, size_t out_size) {
  // Find last path separator (works for both / and \)
  const char *filename = path;
  const char *p = path;
  while (*p) {
    if (*p == '/' || *p == '\\') {
      filename = p + 1;
    }
    p++;
  }

  // Find extension
  const char *ext = strrchr(filename, '.');

  // Copy stem
  size_t len;
  if (ext) {
    len = ext - filename;
  } else {
    len = strlen(filename);
  }

  // Ensure we don't overflow
  if (len >= out_size) {
    len = out_size - 1;
  }

  strncpy(out_stem, filename, len);
  out_stem[len] = '\0';
}

int gmu_read_file(const char *path, char **content, size_t *size) {
  FILE *f = fopen(path, "r");
  size_t _s;
  if (size == NULL)
    size = &_s;
  if (!f)
    return -1;
  fseek(f, 0, SEEK_END);
  *size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = malloc(1 + *size);
  if (!buffer)
    return -5;
  fread(buffer, sizeof(char), *size, f);
  buffer[*size] = '\0';
  *content = buffer;
  return 0;
}
