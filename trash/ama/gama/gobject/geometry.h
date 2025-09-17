#ifndef GAMA_GOBJECT_GEOMETRY_INCLUDED
#define GAMA_GOBJECT_GEOMETRY_INCLUDED
#include "gobject.h"

double GtopY(GObject obj) {
  switch (obj.type) {
  case GTextObject:
    return obj._object.text->pos.y + obj._object.text->fontsize / 2;
  case GSpriteObject:
    return obj._object.sprite->pos.y + obj._object.sprite->size.y / 2;
  case GShapeObject:
    return obj._object.shape->pos.y + obj._object.shape->size.y / 2;
  case GNoObject:
    return 0;
  }
}

double GbottomY(GObject obj) {
  switch (obj.type) {
  case GTextObject:
    return obj._object.text->pos.y - obj._object.text->fontsize / 2;
  case GSpriteObject:
    return obj._object.sprite->pos.y - obj._object.sprite->size.y / 2;
  case GShapeObject:
    return obj._object.shape->pos.y - obj._object.shape->size.y / 2;
  case GNoObject:
    return 0;
  }
}

double GleftX(GObject obj) {
  switch (obj.type) {
  case GTextObject:
    return obj._object.text->pos.x - getTextWidth(obj._object.text) / 2;
  case GSpriteObject:
    return obj._object.sprite->pos.x - obj._object.sprite->size.x / 2;
  case GShapeObject:
    return obj._object.shape->pos.x - obj._object.shape->size.x / 2;
  case GNoObject:
    return 0;
  }
}

double GrightX(GObject obj) {
  switch (obj.type) {
  case GTextObject:
    return obj._object.text->pos.x + getTextWidth(obj._object.text) / 2;
  case GSpriteObject:
    return obj._object.sprite->pos.x + obj._object.sprite->size.x / 2;
  case GShapeObject:
    return obj._object.shape->pos.x + obj._object.shape->size.x / 2;
  case GNoObject:
    return 0;
  }
}

#endif
