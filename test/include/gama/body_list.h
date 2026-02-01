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
 *       original list pointer to prevent memory leaks. Specifically,
 * `gm_ptr_list_pop_at` uses `malloc` internally to create a new list.
 */
#ifndef GM_BODY_LIST_H_INCLUDED
#define GM_BODY_LIST_H_INCLUDED

#include "body.h"
#include <stdlib.h>

#include "ptrlist.h"

// ---------------------------------------------------------------------------
// ---------------------------- Body List Wrapper ----------------------------
// ---------------------------------------------------------------------------

/**
 * @brief A specialized dynamic, NULL-terminated list for `gmBody` pointers.
 *
 * This type is `gmBody**` and provides convenience wrappers around `gmPtrList`
 * for type safety with physics bodies.
 */
typedef gmBody **gmBodies;

/**
 * @brief Calculates the number of elements in a `gmBodies` list.
 * @param list The `gmBodies` list.
 * @return The number of `gmBody*` elements, excluding the NULL terminator.
 */
static inline size_t gm_bodies_length(gmBodies list) {
  return gm_ptr_list_length((gmPtrList)list);
}
/**
 * @brief Checks if a `gmBodies` list is empty.
 * @param list The list to check.
 * @return 1 if the list is NULL or has zero length, 0 otherwise.
 */
static inline int gm_bodies_is_empty(gmBodies list) {
  return gm_ptr_list_is_empty((gmPtrList)list);
}
/**
 * @brief Counts the occurrences of a specific `gmBody` pointer in the list.
 * @param list The list to search.
 * @param obj The `gmBody*` to count.
 * @return The number of times `obj` appears in the list.
 */
static inline size_t gm_bodies_count(gmBodies list, gmBody *obj) {
  return gm_ptr_list_count((gmPtrList)list, obj);
}
/**
 * @brief Adds a `gmBody` pointer to the end of the list.
 * @param list The list to append to.
 * @param obj The `gmBody*` to add.
 * @return A new pointer to the resized list. **The original list pointer
 *         is reallocated and should be replaced by the returned pointer.**
 */
static inline gmBodies gm_bodies_push(gmBodies list, gmBody *obj) {
  return (gmBodies)gm_ptr_list_push((gmPtrList)list, obj);
}
/**
 * @brief Removes the last `gmBody` pointer from the list.
 * @param list The list to modify.
 * @return A new pointer to the resized list. **The original list pointer
 *         is reallocated and should be replaced by the returned pointer.**
 */
static inline gmBodies gm_bodies_pop(gmBodies list) {
  return (gmBodies)gm_ptr_list_pop((gmPtrList)list);
}
/**
 * @brief Removes all occurrences of a specific `gmBody` pointer from the list.
 * @param list The list to modify.
 * @param obj The `gmBody*` to remove.
 * @return A new pointer to the resized list. **The original list pointer
 *         is freed internally and replaced by the returned pointer.**
 */
static inline gmBodies gm_bodies_remove(gmBodies list, gmBody *obj) {
  return (gmBodies)gm_ptr_list_remove((gmPtrList)list, obj);
}
/**
 * @brief Removes a `gmBody` pointer at a specific index.
 * @param list The list to modify.
 * @param idx The index of the `gmBody*` to remove.
 * @return A new pointer to the resized list. **The original list pointer
 *         is freed internally and replaced by the returned pointer.**
 */
static inline gmBodies gm_bodies_pop_at(gmBodies list, size_t idx) {
  return (gmBodies)gm_ptr_list_pop_at((gmPtrList)list, idx);
}
/**
 * @brief Inserts a `gmBody` pointer at a specific index.
 * @param list The list to modify.
 * @param idx The index at which to insert the `gmBody*`.
 * @param value The `gmBody*` to insert.
 * @return A new pointer to the resized list. **The original list pointer
 *         is freed internally and replaced by the returned pointer.**
 */
static inline gmBodies gm_bodies_insert_at(gmBodies list, size_t idx,
                                           gmBody *value) {
  return (gmBodies)gm_ptr_list_insert_at((gmPtrList)list, idx, value);
}
/**
 * @brief Finds the index of a specific `gmBody` pointer.
 * @param list The list to search.
 * @param value The `gmBody*` to find.
 * @return The index of the first occurrence of the value, or -1 if not found.
 */
static inline int gm_bodies_find(gmBodies list, gmBody *value) {
  return gm_ptr_list_find((gmPtrList)list, value);
}
/**
 * @brief Retrieves the `gmBody` pointer at a specific index.
 * @param list The list.
 * @param index The index of the element to retrieve.
 * @return The `gmBody*` at the specified index, or NULL if the index is out of
 * bounds.
 */
static inline gmBody *gm_bodies_get(gmBodies list, size_t index) {
  return (gmBody *)gm_ptr_list_get((gmPtrList)list, index);
}
/**
 * @brief Retrieves the last `gmBody` pointer of the list.
 * @param list The list.
 * @return The last `gmBody*` in the list, or NULL if the list is empty.
 */
static inline gmBody *gm_bodies_last(gmBodies list) {
  return (gmBody *)gm_ptr_list_last((gmPtrList)list);
}
/**
 * @brief Frees the memory used by the `gmBodies` list itself (not the bodies it
 * points to).
 * @param list The list to clear.
 */
static inline void gm_bodies_clear(gmBodies list) {
  gm_ptr_list_clear((gmPtrList)list);
}

/**
 * @brief A macro for iterating over a `gmBodies` list.
 * @param item A `gmBody*` variable to hold the current item.
 * @param list The `gmBodies` list to iterate over.
 */
#define gm_bodies_for_each(item, list)                                         \
  for (size_t i = 0; (list != NULL) && (item = list[i]) != NULL; i++)

#endif // GM_BODY_LIST_H_INCLUDED
