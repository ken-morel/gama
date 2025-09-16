#ifndef GAMA_GLIST_INCLUDED
#define GAMA_GLIST_INCLUDED

#include "gobject.h"

typedef struct {
  GObject *objects;
  int length;
} GList;

GList *GnewList() {
  GList *list = (GList *)malloc(sizeof(GList));
  list->length = 0;
  list->objects = NULL;
  return list;
}
void GinitList(GList *list) {
  list->length = 0;
  list->objects = 0;
}
GObject *GgetItem(GList *list, size_t idx) {
  if (idx >= list->length)
    return NULL;
  else
    return &list->objects[idx];
}
GObject *GsetItem(GList *list, size_t idx, GObject obj) {
  if (list == NULL || idx < 0)
    return NULL;
  if (idx < list->length) {
    list->objects[idx] = obj;
    return &list->objects[idx];
  } else {
    return NULL;
  }
}

void Gappend(GList *list, GObject obj) {
  // realloc on NULL is equivalent to malloc
  GObject *new_objects =
      (GObject *)realloc(list->objects, (list->length + 1) * sizeof(GObject));
  if (new_objects == NULL) {
    // Allocation failed. The original list->objects is still valid.
    // The API doesn't provide a way to signal this error, so we just return.
    return;
  }
  list->objects = new_objects;
  list->objects[list->length] = obj;
  list->length++;
}
int Gpop(GList *list) {
  if (list->length == 0)
    return 0;

  list->length--;

  if (list->length == 0) {
    free(list->objects);
    list->objects = NULL;
    return 1;
  }

  // Try to shrink the allocation.
  GObject *new_objects =
      (GObject *)realloc(list->objects, list->length * sizeof(GObject));

  if (new_objects == NULL) {
    // Shrinking failed. This is rare. The original block is still valid.
    // We restore the length to maintain a consistent state and signal failure.
    list->length++;
    return 0;
  }

  list->objects = new_objects;
  return 1;
}

void GrenderAll(GList *list) {
  int i;
  for (i = 0; i < list->length; i++) {
    Grender(list->objects[i]);
  }
}
void GupdateAll(GList *list, double theta) {
  int i;
  for (i = 0; i < list->length; i++) {
    Gupdate(list->objects[i], theta);
  }
}

#endif
