#ifndef GAMA_COMPRESS_H_INCLUDED
#define GAMA_COMPRESS_H_INCLUDED

#include "../lzav.h"
#include "_malloc.h"
#include <stddef.h>

typedef struct {
  size_t compressed;
  size_t original;
  unsigned char *data;
} gmCompressed;

void gm_compressed_free(gmCompressed *compressed) {
  free(compressed->data);
  memset(compressed, 0, sizeof(*compressed));
}

int gm_compress_to(const unsigned char *data, size_t data_len, unsigned char *dest,
                   size_t dest_len) {
  return lzav_compress_hi(data, dest, data_len, dest_len);
}
size_t gm_compressed_size_guess(size_t data_size) {
  return lzav_compress_bound_hi(data_size);
}

gmCompressed *gm_compress(const unsigned char *data, size_t data_len) {
  gmCompressed *c = calloc(1, sizeof(gmCompressed));
  size_t max_len = gm_compressed_size_guess(data_len);
  unsigned char *dest = malloc(max_len);
  if (!dest)
    return NULL;
  size_t used = gm_compress_to(data, data_len, dest, max_len);
  if (used == 0) {
    free(dest);
    return NULL;
  }
  unsigned char *sized = realloc(dest, used);
  c->compressed = used;
  c->original = data_len;
  c->data = !sized ? dest : sized;
  return c;
}

int gm_decompress_to(const unsigned char *compressed, size_t compressed_len,
                     unsigned char *dest, size_t dest_len) {
  int ret = lzav_decompress(compressed, dest, compressed_len, dest_len);
  // error if ret < -1, so ust return the code
  return ret;
}

unsigned char *gm_decompress(const gmCompressed *compressed) {
  unsigned char *dest = malloc(compressed->original);
  if (!dest)
    return NULL;

  if (0 <= gm_decompress_to(compressed->data, compressed->compressed, dest,
                            compressed->original)) {
    return dest;
  } else {
    free(dest);
    return NULL;
  }
}

#endif // GAMA_COMPRESS_H_INCLUDED
