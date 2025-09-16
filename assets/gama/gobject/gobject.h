#ifndef GAMA_OBJECT_INCLUDED
#define GAMA_OBJECT_INCLUDED

#include "../shape.h"
#include "../sprite.h"
#include "../text.h"

typedef enum {
  GNoObject,
  GShapeObject,
  GTextObject,
  GSpriteObject,
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

GObject newGObject() {
  GObject obj;
  obj._object.shape = NULL;
  obj.type = GNoObject;
  return obj;
}

GObject shapeGObject(Shape *shape) {
  GObject obj = {
      .type = GShapeObject,
      ._object.shape = shape,
  };
  return obj;
}

GObject spriteGObject(Sprite *sprite) {
  GObject obj = {
      .type = GSpriteObject,
      ._object.sprite = sprite,
  };
  return obj;
}

GObject textGObject(Text *text) {
  GObject obj = {
      .type = GTextObject,
      ._object.text = text,
  };
  return obj;
}

void Gupdate(GObject g, double theta) {
  switch (g.type) {
  case GShapeObject:
    return updateShape(g._object.shape, theta);
  case GTextObject:
    return updateText(g._object.text, theta);
  case GSpriteObject:
    return updateSprite(g._object.sprite, theta);
  case GNoObject:
    return;
  }
}

int Grender(GObject g) {
  switch (g.type) {
  case GShapeObject:
    return renderShape(g._object.shape);
  case GTextObject:
    return renderText(g._object.text);
  case GSpriteObject:
    return renderSprite(g._object.sprite);
  case GNoObject:
    return 0;
  }
}

int Gdestroy(GObject g) {
  switch (g.type) {
  case GShapeObject:
    free(g._object.shape);
    break;
  case GTextObject:
    free(g._object.text);
    break;
  case GSpriteObject:
    free(g._object.sprite);
    break;
  case GNoObject:
    return 0;
  }
  g.type = GNoObject;
  return 1;
}

#endif
