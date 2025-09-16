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
  if (list->objects == NULL || list->length == 0) {
    list->objects = &obj;
    list->length = 1;
  } else {
    list->objects =
        (GObject *)realloc(list->objects, (list->length + 1) * sizeof(GObject));
    list->objects[list->length++] = obj;
  }
}
int Gpop(GList *list) {
  if (list->length == 0)
    return 0;
  GObject obj = list->objects[--list->length];
  list->objects =
      (GObject *)realloc(list->objects, list->length * sizeof(GObject));
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
