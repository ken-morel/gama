#pragma once

#include <stddef.h>
static inline int abs(int a) { return a > 0 ? a : -a; }
static inline double fabs(double a) { return a > 0 ? a : -a; }

#ifndef MEMORY
#define MEMORY 10
#endif

#ifndef MEMORY_B
#define MEMORY_B 0
#endif

#define MEMORY_TOTAL ((MEMORY << 20) + MEMORY_B)

#ifndef MEMORY_SPOTS
#define MEMORY_SPOTS (MEMORY_TOTAL / 10)
#endif

struct _memory_spot {
  size_t index; // array index, could be pointer
  size_t size;  // size of memory spot, it's zero for unused memory
};

char _memory[MEMORY_TOTAL];

struct _memory_spot _memory_spots[MEMORY_SPOTS];
size_t _memory_spot_size = 0; // the next memory spot

void _remove_memory_spot(size_t index) {
  for (size_t i = index; i < _memory_spot_size - 1; i++) {
    _memory_spots[i] = _memory_spots[i + 1];
  }
  _memory_spot_size--;
}

struct _memory_spot _add_memory_spot(size_t index, size_t size) {
  if (index == 0 && _memory_spot_size > 0) {
    size_t last_memory = _memory_spots[_memory_spot_size - 1].index +
                         _memory_spots[_memory_spot_size - 1].size;
    return _add_memory_spot(last_memory, size);
  }
  size_t mem_index = 0;
  size_t found = 0;
  for (size_t i = 0; i < MEMORY_SPOTS; i++) {
    if (_memory_spots[i].index >
        index) { // memory which goes after it has to be moved forward
      for (size_t j = _memory_spot_size; j > i; j--)
        _memory_spots[j] = _memory_spots[j - 1];
      // now spot i is free
      mem_index = i;
      found = 1;
      break;
    }
  }
  if (!found) {
    mem_index = _memory_spot_size;
    _memory_spot_size++;
  }
  _memory_spots[mem_index].index = index;
  _memory_spots[mem_index].size = size;
  return _memory_spots[mem_index];
}

void *malloc(size_t size) {
  if (size == 0)
    return NULL;
  for (size_t i = 0; i < _memory_spot_size - 1;
       i++) {                         // the -1 is safety for the i+1
    if (_memory_spots[i].size == 0) { // free memory, take and split the rest
      size_t free_size = _memory_spots[i + 1].index - _memory_spots[i].index;
      if (free_size >= size) {
        _memory_spots[i].size = size;
        if (free_size > size)
          _add_memory_spot(i + size, 0); // free remaining space
        return (void *)_memory_spots[i].index;
      }
    }
  }
  struct _memory_spot spot = _add_memory_spot(0, size);
  return (void *)spot.index;
}

void _flatten_empty_memory_spots() {
  size_t done;
  do {
    done = 0;
    for (size_t i = 0; i < _memory_spot_size - 2; i++) {
      if (_memory_spots[i].size == 0 && _memory_spots[i + 1].size == 0) {
        _remove_memory_spot(i + 1);
        done++;
      }
    }
  } while (done > 0);
}

void free(void *ptr) {
  size_t index = (size_t)ptr;
  for (size_t i = 0; i < _memory_spot_size; i++) {
    if (_memory_spots[i].index == index) {
      _memory_spots[i].size = 0;
    }
  }
  _flatten_empty_memory_spots();
}
