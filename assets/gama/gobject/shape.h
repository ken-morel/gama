#ifndef GAMA_GSHAPE_INCLUDED
#define GAMA_GSHAPE_INCLUDED
#include "gobject.h"

GObject GRectangle(Pos pos, Pos size, Color color) {
  GObject obj = Gnew();
  obj._object.shape = newShape();
  if (obj._object.shape == NULL)
    return obj; // Return GNoObject
  if (initRectangle(obj._object.shape, pos, size, color) == 1) {
    obj.type = GShapeObject;
    return obj;
  } else {
    free(obj._object.shape);
    return obj;
  }
}

GObject GCircle(Pos pos, double radius, Color color) {
  GObject obj = Gnew();
  obj._object.shape = newShape();
  if (obj._object.shape == NULL)
    return obj; // Return GNoObject
  if (initCircle(obj._object.shape, pos, radius, color) == 1) {
    obj.type = GShapeObject;
    return obj;
  } else {
    free(obj._object.shape);
    return obj;
  }
}

GObject GText(Font *font, Pos pos, double size, const char *text) {
  GObject obj = Gnew();
  obj._object.text = createText(text, size, font, pos);
  if (obj._object.text == NULL)
    return obj; // Return GNoObject
  else
    obj.type = GTextObject;
  return obj;
}

GObject GSprite(const char *path, size_t width, size_t height, Pos pos,
                Pos size) {
  GObject obj = Gnew();
  obj._object.sprite = createSprite(path, width, height, pos, size);
  if (obj._object.sprite == NULL)
    return obj; // Return GNoObject
  else
    obj.type = GSpriteObject;
  return obj;
}

#endif
