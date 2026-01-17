/**
 * @file str.h
 * @brief Defines a dynamic string structure and provides utility functions for its manipulation.
 *
 * This file offers a simple dynamic string (`gmStr`) implementation that can
 * grow as needed, making it suitable for building strings without fixed-size
 * buffer limitations.
 */
#pragma once

#include "_malloc.h" // Assuming this is where custom malloc/realloc are defined

#include <stddef.h>
#include <string.h>
/**
 * @brief Represents a dynamic, heap-allocated string.
 *
 * This structure stores both the current length of the string and a pointer
 * to its content, allowing for efficient string manipulation and growth.
 */
typedef struct {
  size_t length; /**< The current length of the string (excluding null terminator). */
  char *content; /**< Pointer to the heap-allocated string content (null-terminated). */
} gmStr;

/**
 * @brief Initializes an empty `gmStr` struct.
 * @return A new `gmStr` instance with length 0 and NULL content.
 */
gmStr gm_str() { return (gmStr){.length = 0, .content = NULL}; }

/**
 * @brief Appends a specified number of characters from a C string to a `gmStr`.
 *
 * This function reallocates memory for the `gmStr`'s content if necessary
 * to accommodate the new characters.
 *
 * @param s A pointer to the `gmStr` to append to.
 * @param n The number of characters from `txt` to append.
 * @param txt The null-terminated C string to append.
 * @return 0 on success, -1 on invalid input, -5 on reallocation failure.
 */
int gm_str_appendn(gmStr *s, size_t n, const char *txt) {
  if (!s || !txt || n == 0)
    return -1;
  if (s->length == 0) {
    s->length = 0;
    s->content = malloc(n + 1);
    s->content[n] = '\0'; // Ensure null-termination
  } else {
    size_t newcap = s->length + n + 1;
    char *newtxt = realloc(s->content, newcap);
    newtxt[newcap - 1] = '\0';
    if (!newtxt)
      return -5;
    s->content = newtxt;
  }
  for (size_t i = 0; i < n; i++) {
    s->content[s->length] = txt[i];
    s->length++;
  }
  return 0;
}

/**
 * @brief Appends a null-terminated C string to a `gmStr`.
 * @param s A pointer to the `gmStr` to append to.
 * @param txt The null-terminated C string to append.
 * @return 0 on success, or an error code from `gm_str_appendn`.
 */
static inline int gm_str_append(gmStr *s, const char *txt) {
  return gm_str_appendn(s, strlen(txt), txt);
}

/**
 * @brief Frees the heap-allocated content of a `gmStr` and resets its length to 0.
 *
 * The `gmStr` struct itself is not freed, only its internal buffer.
 *
 * @param str A pointer to the `gmStr` to clear.
 */
void gm_str_clear(gmStr *str) {
  if (str && str->content) {
    free(str->content);
    str->content = NULL;
  }
  if (str) {
    str->length = 0;
  }
}
