#ifndef GAMA_OBJECT_INCLUDED
#define GAMA_OBJECT_INCLUDED

#include "shape.h"
#include "sprite.h"
#include "text.h"
#include <cstdlib>

typedef enum {
  GShapeObject,
  GTextObject,
  GSpriteObject,
  GNoObject,
} GObjectType;
typedef union {
  Sprite *sprite;
  Text *text;
  Shape *shape;
} _GObject; // Actually all the same size
typedef struct {
  _GObject _object;
  GObjectType type;
} GObject;

GObject *newGObject() {
  GObject *obj = (GObject *)malloc(sizeof(GObject));
  obj->_object.shape = NULL; // all have same salue;
  return obj;
}

GObject *shapeGObject(Shape *shape) {
  GObject *obj = newGObject();
  obj->_object.shape = shape;
  obj->type = GShapeObject;
  return obj;
}

GObject *spriteGObject(Sprite *sprite) {
  GObject *obj = newGObject();
  obj->_object.sprite = sprite;
  obj->type = GSpriteObject;
  return obj;
}

GObject *textGObject(Text *text) {
  GObject *obj = newGObject();
  obj->_object.text = text;
  obj->type = GTextObject;
  return obj;
}

void updateGObject(GObject *g, double theta) {
  switch (g->type) {
  case GShapeObject:
    return updateShape(g->_object.shape, theta);
  case GTextObject:
    return updateText(g->_object.text, theta);
  case GSpriteObject:
    return updateSprite(g->_object.sprite, theta);
  default:
    return;
  }
}

void renderGObject(GObject *g) {
  switch (g->type) {
  case GShapeObject:
    return renderShape(g->_object.shape);
  case GTextObject:
    return renderText(g->_object.text);
  case GSpriteObject:
    return renderSprite(g->_object.sprite);
  default:
    return;
  }
}

typedef struct {
  GObject **objects;
  int length;
} GObjectList;

GObjectList *newGObjectList() {
  GObjectList *list = (GObjectList *)malloc(sizeof(GObjectList));
  list->length = 0;
  list->objects = NULL;
  return list;
}
void createGObjectList(GObjectList *list) {
  list->objects = NULL;
  list->length = 0;
}
GObject *getGObjectFromList(GObjectList *list, unsigned int idx) {
  if (idx >= list->length)
    return NULL;
  else
    return list->objects[idx];
}

void addGObjectToList(GObjectList *list, GObject *obj) {
  if (list->objects == NULL || list->length == 0) {
    list->objects = &obj;
    list->length = 1;
  } else {
    list->objects = (GObject **)realloc(list->objects,
                                        (list->length + 1) * sizeof(GObject *));
    list->objects[list->length] = obj;
  }
}
GObject *popGObjectFromList(GObjectList *list) {
  if (list->length == 0)
    return NULL;
  GObject *obj = list->objects[--list->length];
  list->objects =
      (GObject **)realloc(list->objects, list->length * sizeof(GObject *));
  return obj;
}

void renderGObjects(GObjectList *list) {
  int i;
  for (i = 0; i < list->length; i++) {
    renderGObject(list->objects[i]);
  }
}
void updateGObjects(GObjectList *list, double theta) {
  int i;
  for (i = 0; i < list->length; i++) {
    updateGObject(list->objects[i], theta);
  }
}
#endif
