/**
 * @file body_list.h
 * @brief Provides a dynamic, NULL-terminated pointer list implementation.
 *
 * This file contains a generic pointer list (`gmPtrList`) and a specialized
 * version for physics bodies (`gmBodies`). The lists automatically resize and
 * are always NULL-terminated, making them easy to iterate.
 *
 * @note The functions in this file that return a new list (e.g., push, pop)
 *       allocate new memory. It is the caller's responsibility to free the
 *       original list pointer to prevent memory leaks.
 */
#pragma once

#include "body.h"
#include <stdlib.h>

// ---------------------------------------------------------------------------
// --------------------------- Generic Pointer List --------------------------
// ---------------------------------------------------------------------------

/**
 * @brief A dynamic, NULL-terminated array of generic pointers.
 */
typedef void **gmPtrList;

/**
 * @brief Calculates the number of elements in a pointer list.
 * @param list The NULL-terminated pointer list.
 * @return The number of elements, excluding the NULL terminator.
 */
size_t gm_ptr_list_length(gmPtrList list) {
  if (list == NULL)
    return 0;
  size_t i = 0;
  while (list[i] != NULL)
    i++;
  return i;
}

/**
 * @brief Checks if a pointer list is empty.
 * @param list The list to check.
 * @return 1 if the list is NULL or has zero length, 0 otherwise.
 */
int gm_ptr_list_is_empty(gmPtrList list) {
  return list == NULL || list[0] == NULL;
}

/**
 * @brief Counts the occurrences of a specific pointer in the list.
 * @param list The list to search.
 * @param obj The pointer to count.
 * @return The number of times `obj` appears in the list.
 */
size_t gm_ptr_list_count(gmPtrList list, void *obj) {
  if (list == NULL)
    return 0;
  size_t count = 0;
  for (size_t i = 0; list[i] != NULL; i++) {
    if (list[i] == obj)
      count++;
  }
  return count;
}

/**
 * @brief Adds a pointer to the end of the list.
 * @param list The list to append to.
 * @param obj The pointer to add.
 * @return A new pointer to the resized list. The original list should be freed.
 */
gmPtrList gm_ptr_list_push(gmPtrList list, void *obj) {
  size_t length = gm_ptr_list_length(list);
  gmPtrList new_list = realloc(list, (length + 2) * sizeof(void *));
  new_list[length] = obj;
  new_list[length + 1] = NULL;
  return new_list;
}

/**
 * @brief Removes the last element from the list.
 * @param list The list to modify.
 * @return A new pointer to the resized list. The original list should be freed.
 */
gmPtrList gm_ptr_list_pop(gmPtrList list) {
  size_t len = gm_ptr_list_length(list);
  if (len == 0)
    return list;
  gmPtrList new_list = realloc(list, len * sizeof(void *));
  new_list[len - 1] = NULL;
  return new_list;
}

/**
 * @brief Removes all occurrences of a specific pointer from the list.
 * @param list The list to modify.
 * @param obj The pointer to remove.
 * @return A new pointer to the resized list. The original list should be freed.
 */
gmPtrList gm_ptr_list_remove(gmPtrList list, void *obj) {
  size_t len = gm_ptr_list_length(list);
  size_t count = gm_ptr_list_count(list, obj);
  if (count == 0)
    return list;

  gmPtrList new_list = malloc((len - count + 1) * sizeof(void *));
  size_t index = 0;
  for (size_t i = 0; i < len; i++) {
    if (list[i] != obj) {
      new_list[index++] = list[i];
    }
  }
  new_list[index] = NULL;
  free(list);
  return new_list;
}

/**
 * @brief Removes an element at a specific index.
 * @param list The list to modify.
 * @param idx The index of the element to remove.
 * @return A new pointer to the resized list. The original list should be freed.
 */
gmPtrList gm_ptr_list_pop_at(gmPtrList list, size_t idx) {
  size_t len = gm_ptr_list_length(list);
  if (idx >= len)
    return list;

  gmPtrList new_list = malloc(len * sizeof(void *));
  size_t new_idx = 0;
  for (size_t i = 0; i < len; i++) {
    if (i != idx) {
      new_list[new_idx++] = list[i];
    }
  }
  new_list[new_idx] = NULL;
  free(list);
  return new_list;
}

/**
 * @brief Inserts a pointer at a specific index.
 * @param list The list to modify.
 * @param idx The index at which to insert the value.
 * @param value The pointer to insert.
 * @return A new pointer to the resized list. The original list should be freed.
 */
gmPtrList gm_ptr_list_insert_at(gmPtrList list, size_t idx, void *value) {
  size_t len = gm_ptr_list_length(list);
  if (idx > len)
    idx = len;

  gmPtrList new_list = malloc((len + 2) * sizeof(void *));
  for (size_t i = 0; i < idx; i++) {
    new_list[i] = list[i];
  }
  new_list[idx] = value;
  for (size_t i = idx; i < len; i++) {
    new_list[i + 1] = list[i];
  }
  new_list[len + 1] = NULL;
  free(list);
  return new_list;
}

/**
 * @brief Finds the index of a specific pointer.
 * @param list The list to search.
 * @param value The pointer to find.
 * @return The index of the first occurrence of the value, or -1 if not found.
 */
int gm_ptr_list_find(gmPtrList list, void *value) {
  if (list == NULL)
    return -1;
  for (size_t i = 0; list[i] != NULL; i++) {
    if (list[i] == value)
      return i;
  }
  return -1;
}

/**
 * @brief Retrieves the element at a specific index.
 * @param list The list.
 * @param index The index of the element to retrieve.
 * @return The pointer at the specified index, or NULL if the index is out of
 * bounds.
 */
void *gm_ptr_list_get(gmPtrList list, size_t index) {
  if (list == NULL || index >= gm_ptr_list_length(list)) {
    return NULL;
  }
  return list[index];
}

/**
 * @brief Retrieves the last element of the list.
 * @param list The list.
 * @return The last pointer in the list, or NULL if the list is empty.
 */
void *gm_ptr_list_last(gmPtrList list) {
  if (gm_ptr_list_is_empty(list))
    return NULL;
  return list[gm_ptr_list_length(list) - 1];
}

/**
 * @brief Frees the memory used by the list.
 * @param list The list to clear.
 */
void gm_ptr_list_clear(gmPtrList list) {
  if (list != NULL) {
    free(list);
  }
}

/**
 * @brief A macro for iterating over a gmPtrList.
 * @param item The variable to hold the current item (e.g., `gmBody* item`).
 * @param list The gmPtrList to iterate over.
 */
#define gm_ptr_list_for_each(item, list)                                       \
  for (size_t i = 0; (list != NULL) && (item = list[i]) != NULL; i++)

// ---------------------------------------------------------------------------
// ---------------------------- Body List Wrapper ----------------------------
// ---------------------------------------------------------------------------

/**
 * @brief A specialized pointer list for `gmBody` pointers.
 */
typedef gmBody **gmBodies;

static inline size_t gm_bodies_length(gmBodies list) {
  return gm_ptr_list_length((gmPtrList)list);
}
static inline int gm_bodies_is_empty(gmBodies list) {
  return gm_ptr_list_is_empty((gmPtrList)list);
}
static inline size_t gm_bodies_count(gmBodies list, gmBody *obj) {
  return gm_ptr_list_count((gmPtrList)list, obj);
}
static inline gmBodies gm_bodies_push(gmBodies list, gmBody *obj) {
  return (gmBodies)gm_ptr_list_push((gmPtrList)list, obj);
}
static inline gmBodies gm_bodies_pop(gmBodies list) {
  return (gmBodies)gm_ptr_list_pop((gmPtrList)list);
}
static inline gmBodies gm_bodies_remove(gmBodies list, gmBody *obj) {
  return (gmBodies)gm_ptr_list_remove((gmPtrList)list, obj);
}
static inline gmBodies gm_bodies_pop_at(gmBodies list, size_t idx) {
  return (gmBodies)gm_ptr_list_pop_at((gmPtrList)list, idx);
}
static inline gmBodies gm_bodies_insert_at(gmBodies list, size_t idx,
                                           gmBody *value) {
  return (gmBodies)gm_ptr_list_insert_at((gmPtrList)list, idx, value);
}
static inline int gm_bodies_find(gmBodies list, gmBody *value) {
  return gm_ptr_list_find((gmPtrList)list, value);
}
static inline gmBody *gm_bodies_get(gmBodies list, size_t index) {
  return (gmBody *)gm_ptr_list_get((gmPtrList)list, index);
}
static inline gmBody *gm_bodies_last(gmBodies list) {
  return (gmBody *)gm_ptr_list_last((gmPtrList)list);
}
static inline void gm_bodies_clear(gmBodies list) {
  gm_ptr_list_clear((gmPtrList)list);
}

/**
 * @brief A macro for iterating over a gmBodies list.
 * @param item A `gmBody*` variable to hold the current item.
 * @param list The gmBodies list to iterate over.
 */
#define gm_bodies_for_each(item, list)                                         \
  for (size_t i = 0; (list != NULL) && (item = list[i]) != NULL; i++)
