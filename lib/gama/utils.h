/**
 * @file utils.h
 * @brief Provides general utility functions for file handling and string manipulation.
 *
 * This file contains miscellaneous helper functions that are not directly
 * related to a specific Gama module but are useful across the engine.
 */
#ifndef GM_UTILS_H_INCLUDED
#define GM_UTILS_H_INCLUDED
#include "_malloc.h" // For custom malloc/free
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Extracts the base filename (filename with extension) from a full path.
 *
 * Example: `path = "/home/user/image.png"` -> `out_base = "image.png"`
 *
 * @param path The full path string.
 * @param out_base Buffer to store the extracted filename.
 * @param out_size The size of the `out_base` buffer.
 */
void gmu_get_filename_base(const char *path, char *out_base, size_t out_size) {
  // Find last path separator (works for both / and \)
  const char *filename = path;
  const char *p = path;
  while (*p) {
    if (*p == '/' || *p == '\\') {
      filename = p + 1;
    }
    p++;
  }

  // Find extension (not used for base, but can be for stem)
  // const char *ext = strrchr(filename, '.');

  size_t len = strlen(filename);

  // Ensure we don't overflow out_base
  if (len >= out_size)
    len = out_size - 1;

  strncpy(out_base, filename, len);
  out_base[len] = '\0';
}

/**
 * @brief Extracts the filename stem (filename without extension) from a full path.
 *
 * Example: `path = "/home/user/image.png"` -> `out_stem = "image"`
 *
 * @param path The full path string.
 * @param out_stem Buffer to store the extracted filename stem.
 * @param out_size The size of the `out_stem` buffer.
 */
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

/**
 * @brief Reads the entire content of a file into a dynamically allocated buffer.
 *
 * The caller is responsible for freeing the `*content` buffer.
 *
 * @param path The path to the file to read.
 * @param content A pointer to a `char*` that will be allocated and filled with the file's content.
 * @param size A pointer to a `size_t` that will store the size of the read content. Can be NULL.
 * @return 0 on success, -1 if the file cannot be opened, -5 on memory allocation failure.
 */
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

/**
 * @internal
 * @brief Copies a string up to an End-Of-Line character or maximum length, trimming trailing whitespace.
 * @param dest The destination buffer.
 * @param src The source string.
 * @param max_len The maximum length of the destination buffer, including null terminator.
 */
static inline void gm3u_str_copy_eol(char *dest, const char *src,
                                     size_t max_len) {
  while (*src && isspace((unsigned char)*src))
    src++;
  size_t i = 0;
  while (src[i] != '\0' && src[i] != '\n' && src[i] != '\r' &&
         i < max_len - 1) {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
  while (i > 0 && isspace((unsigned char)dest[i - 1])) {
    dest[--i] = '\0';
  }
}

#endif // GM_UTILS_H_INCLUDED
