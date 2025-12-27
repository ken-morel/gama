#pragma once

#ifndef GM_MALLOC
#define GM_MALLOC
#endif

#include "gapi.h"
#include <stddef.h>
#ifndef MEMORY
// default memory to 10MB
#define MEMORY 10

#endif
#ifndef MEMORY_B
#define MEMORY_B 0
#endif
#define MEMORY_TOTAL ((MEMORY << 20) + MEMORY_B)
#ifndef MEMORY_SPOTS
#define MEMORY_SPOTS (MEMORY_TOTAL / 100)
#endif

#define _MALLOC_H 1

struct _memory_spot {
  size_t index; // Start index in memory pool
  size_t size;  // Size of this block (0 = free)
};
// Main memory pool and bookkeeping
static char _memory[MEMORY_TOTAL];
static struct _memory_spot _memory_spots[MEMORY_SPOTS];
static size_t _memory_spot_size = 0;
static void _remove_memory_spot(size_t index) {
  for (size_t i = index; i < _memory_spot_size - 1; i++) {
    _memory_spots[i] = _memory_spots[i + 1];
  }
  if (_memory_spot_size > 0) {
    _memory_spot_size--;
  }
}
static struct _memory_spot _add_memory_spot(size_t index, size_t size) {
  if (_memory_spot_size >= MEMORY_SPOTS) {
    gapi_log("OOM: sorry kid, memory's finish, no _spots left, try "
             "https://gama.rbs.cm/faq#oom");
    gapi_quit();
    return (struct _memory_spot){0, 0};
  }
  // Find new spot
  size_t insert_pos = _memory_spot_size;
  for (size_t i = 0; i < _memory_spot_size; i++) {
    if (_memory_spots[i].index > index) {
      // move others right to keep order and ease searching
      for (size_t j = _memory_spot_size; j > i; j--) {
        _memory_spots[j] = _memory_spots[j - 1];
      }
      insert_pos = i;
      break;
    }
  }
  // spot found, use it
  _memory_spots[insert_pos].index = index;
  _memory_spots[insert_pos].size = size;
  if (insert_pos == _memory_spot_size) // last spot => new spot
    _memory_spot_size++;

  return _memory_spots[insert_pos];
}
void *malloc(size_t size) {
  if (size == 0)
    return NULL;
  if (_memory_spot_size == 0) {
    // same as add_memory_spot will do
    _memory_spots[0].index = 0;
    _memory_spots[0].size = 0; // 0, free for now
    _memory_spot_size = 1;
  }
  // Look for a free spot that's large enough
  for (size_t i = 0; i < _memory_spot_size; i++) {
    if (_memory_spots[i].size == 0) { // Free block
      size_t start = _memory_spots[i].index;
      size_t end = (i + 1 < _memory_spot_size) ? _memory_spots[i + 1].index
                                               : MEMORY_TOTAL;
      size_t available_size = end - start;
      if (available_size >= size) {
        _memory_spots[i].size = size; // Mark as used
        // If there's leftover space, split it
        if (available_size > size) {
          _add_memory_spot(start + size, 0); // New free block
        }
        return &_memory[start];
      }
    }
  }
  // No suitable block found, create one
  if (_memory_spot_size > 0) {
    struct _memory_spot *last = &_memory_spots[_memory_spot_size - 1];
    size_t end_of_last = last->index + last->size;
    if (end_of_last + size < MEMORY_TOTAL) {
      struct _memory_spot new_spot = _add_memory_spot(end_of_last, size);
      return &_memory[new_spot.index];
    }
  }
  return NULL; // Out of memory
}
void free(void *ptr) {
  if (!ptr)
    return;
  char *char_ptr = (char *)ptr;
  size_t index = char_ptr - _memory;
  if (index >= MEMORY_TOTAL)
    return; // Invalid pointer
  // Find the block this pointer belongs to
  for (size_t i = 0; i < _memory_spot_size; i++) {
    if (_memory_spots[i].index == index && _memory_spots[i].size > 0) {
      _memory_spots[i].size = 0; // Mark as free
      return;
    }
  }
}
void *calloc(size_t count, size_t size) {
  size_t total_size = count * size;
  void *ptr = malloc(total_size);
  if (ptr) {
    char *p = (char *)ptr;
    for (size_t i = 0; i < total_size; i++) {
      p[i] = 0; // zero all block contents
    }
  }
  return ptr;
}
void *realloc(void *ptr, size_t size) {
  if (!ptr)
    return malloc(size);
  if (size == 0) {
    free(ptr);
    return NULL;
  }
  // Find current block size
  char *char_ptr = (char *)ptr;
  size_t index = char_ptr - _memory;
  for (size_t i = 0; i < _memory_spot_size; i++) {
    if (_memory_spots[i].index == index && _memory_spots[i].size > 0) {
      if (_memory_spots[i].size >= size) {
        // Current block is large enough
        return ptr;
      } else {
        // Need to allocate new block and copy
        void *new_ptr = malloc(size);
        if (new_ptr) {
          // Copy old data
          char *src = (char *)ptr;
          char *dst = (char *)new_ptr;
          for (size_t j = 0; j < _memory_spots[i].size; j++) {
            dst[j] = src[j];
          }
          free(ptr);
        }
        return new_ptr;
      }
    }
  }
  return NULL; // Invalid pointer
}
