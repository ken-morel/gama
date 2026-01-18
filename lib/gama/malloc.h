/**
 * @file malloc.h
 * @brief Custom memory allocation functions (`malloc`, `free`, `calloc`, `realloc`)
 *        using a fixed-size static memory pool.
 *
 * This file provides an alternative memory management system for Gama,
 * primarily for environments where dynamic system `malloc` might be
 * unavailable or undesirable (e.g., embedded systems, WebAssembly with specific
 * memory requirements). It pre-allocates a large static buffer and manages
 * memory chunks within it.
 *
 * @warning This implementation redefines standard C library functions (`malloc`,
 *          `free`, `calloc`, `realloc`). Care must be taken to ensure this
 *          does not conflict with system-level memory allocation or other
 *          libraries that expect standard `libc` behavior. This file is
 *          typically included conditionally.
 */
#ifndef GM_MALLOC_H_INCLUDED
#define GM_MALLOC_H_INCLUDED
#include <stdlib.h>
#ifndef GM_MALLOC
#define GM_MALLOC
#endif

#include <stddef.h>
/**
 * @def MEMORY
 * @internal
 * @brief Defines the size of the main memory pool in megabytes.
 * Default is 10MB if not otherwise defined.
 */
#ifndef MEMORY
// default memory to 10MB
#define MEMORY 10

#endif
/**
 * @def MEMORY_B
 * @internal
 * @brief Defines an additional size for the memory pool in bytes.
 * Default is 0 bytes if not otherwise defined.
 */
#ifndef MEMORY_B
#define MEMORY_B 0
#endif
/**
 * @def MEMORY_TOTAL
 * @internal
 * @brief Total size of the memory pool in bytes.
 */
#define MEMORY_TOTAL ((MEMORY << 20) + MEMORY_B)
/**
 * @def MEMORY_SPOTS
 * @internal
 * @brief Maximum number of memory blocks (spots) that can be tracked.
 * Calculated as `MEMORY_TOTAL / 100`.
 */
#ifndef MEMORY_SPOTS
#define MEMORY_SPOTS (MEMORY_TOTAL / 100)
#endif

#define _MALLOC_H 1

/**
 * @internal
 * @brief Represents a block of memory within the static pool.
 */
struct _memory_spot {
  size_t index; /**< Starting index (offset) in the `_memory` pool. */
  size_t size;  /**< Size of this block (0 indicates a free block). */
};
/**
 * @internal
 * @brief The main static memory pool buffer.
 */
static char _memory[MEMORY_TOTAL];
/**
 * @internal
 * @brief Array to keep track of allocated and free memory spots.
 */
static struct _memory_spot _memory_spots[MEMORY_SPOTS];
/**
 * @internal
 * @brief Current number of active memory spots being tracked.
 */
static size_t _memory_spot_size = 0;
/**
 * @internal
 * @brief Removes a memory spot from the `_memory_spots` array.
 * @param index The index of the spot to remove.
 */
static void _remove_memory_spot(size_t index) {
  for (size_t i = index; i < _memory_spot_size - 1; i++) {
    _memory_spots[i] = _memory_spots[i + 1];
  }
  if (_memory_spot_size > 0) {
    _memory_spot_size--;
  }
}
/**
 * @internal
 * @brief Adds a new memory spot to the `_memory_spots` array, maintaining sorted order.
 * @param index The starting index of the new spot.
 * @param size The size of the new spot.
 * @return The newly added `_memory_spot` struct.
 */
static struct _memory_spot _add_memory_spot(size_t index, size_t size) {
  if (_memory_spot_size >= MEMORY_SPOTS) {
    // gapi_log("OOM: sorry kid, memory's finish, no _spots left, try "
    // "https://gama.rbs.cm/faq#oom");
    exit(100); // Exits if no more spots are available
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

/**
 * @brief Custom implementation of `malloc` using a static memory pool.
 *
 * Allocates a block of `size` bytes from the predefined static memory pool.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory block, or `NULL` if allocation fails
 *         (e.g., out of memory or no suitable spot).
 */
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

/**
 * @brief Custom implementation of `free` for memory allocated by `malloc` (this custom version).
 *
 * Frees a previously allocated memory block, making it available for future allocations.
 *
 * @param ptr A pointer to the memory block to free. If `ptr` is `NULL`, no operation is performed.
 */
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

/**
 * @brief Custom implementation of `calloc` using a static memory pool.
 *
 * Allocates a block of memory for an array of `count` elements, each of `size` bytes,
 * and initializes all bytes in the allocated block to zero.
 *
 * @param count The number of elements to allocate.
 * @param size The size of each element in bytes.
 * @return A pointer to the allocated and zero-initialized memory, or `NULL` if allocation fails.
 */
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

/**
 * @brief Custom implementation of `realloc` for memory allocated by `malloc` (this custom version).
 *
 * Resizes a previously allocated memory block.
 *
 * @param ptr A pointer to the memory block to reallocate. If `ptr` is `NULL`, behaves like `malloc`.
 * @param size The new size for the memory block. If `size` is `0`, behaves like `free`.
 * @return A pointer to the reallocated memory block, or `NULL` if reallocation fails.
 */
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

#endif // GM_MALLOC_H_INCLUDED
