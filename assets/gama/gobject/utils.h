#ifndef GAMA_GOBJECT_UTILS_H
#define GAMA_GOBJECT_UTILS_H

#include "gobject.h"

// -------------- set potision --------------------
int GsetPosition(GObject obj, Pos pos) {
  switch (obj.type) {
  case GShapeObject:
    return setShapePosition(obj._object.shape, &pos);
  case GTextObject:
    return setTextPosition(obj._object.text, &pos);
  case GSpriteObject:
    return setSpritePosition(obj._object.sprite, &pos);
  case GNoObject:
    return -5;
  }
}
int GsetVelocity(GObject obj, Pos vel) {
  switch (obj.type) {
  case GShapeObject:
    return setShapeVelocity(obj._object.shape, &vel);
  case GTextObject:
    return setTextVelocity(obj._object.text, &vel);
  case GSpriteObject:
    return setSpriteVelocity(obj._object.sprite, &vel);
  case GNoObject:
    return -5;
  }
}

int GsetAcceleration(GObject obj, Pos acc) {
  switch (obj.type) {
  case GShapeObject:
    return setShapeAcceleration(obj._object.shape, &acc);
  case GTextObject:
    return setTextAcceleration(obj._object.text, &acc);
  case GSpriteObject:
    return setSpriteAcceleration(obj._object.sprite, &acc);
  case GNoObject:
    return -5;
  }
}

// -------------- get potision --------------------
Pos GgetPosition(GObject obj) {
  switch (obj.type) {
  case GShapeObject:
    return getShapePosition(obj._object.shape);
  case GTextObject:
    return getTextPosition(obj._object.text);
  case GSpriteObject:
    return getSpritePosition(obj._object.sprite);
  case GNoObject:
    return at(0, 0);
  }
}
Pos GgetVelocity(GObject obj) {
  switch (obj.type) {
  case GShapeObject:
    return getShapeVelocity(obj._object.shape);
  case GTextObject:
    return getTextVelocity(obj._object.text);
  case GSpriteObject:
    return getSpriteVelocity(obj._object.sprite);
  case GNoObject:
    return at(0, 0);
  }
}

Pos GgetAcceleration(GObject obj) {
  switch (obj.type) {
  case GShapeObject:
    return getShapeAcceleration(obj._object.shape);
  case GTextObject:
    return getTextAcceleration(obj._object.text);
  case GSpriteObject:
    return getSpriteAcceleration(obj._object.sprite);
  case GNoObject:
    return at(0, 0);
  }
}

#endif // GAMA_GOBJECT_UTILS_H
