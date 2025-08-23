#ifndef GAMA_SHAPE_INCLUDED
#define GAMA_SHAPE_INCLUDED

#include "color.h"
#include "vector.h"

#include "drawshape.h"

typedef enum {
  RectangleShape,
  CircleShape,
} ShapeType;
typedef struct {
  ShapeType type;
  Vector *pos;
  Pos *size;
  Color color;
  float radius;
} Shape;

Shape *createRectangle(Pos *pos, Pos *size, Color col) {

  Shape *s = (Shape *)malloc(sizeof(Shape));
  s->type = RectangleShape;
  s->pos = vectorAt(pos);
  s->color = col;
  s->radius = 0;
  s->size = size;

  return s;
}
Shape *createCircle(Pos *pos, float radius, Color col) {

  Shape *s = (Shape *)malloc(sizeof(Shape));
  s->type = CircleShape;
  s->pos = vectorAt(pos);
  s->color = col;
  s->radius = radius;
  s->size = at(0, 0);

  return s;
}

void updateShape(Shape *s, float theta) { updateVector(s->pos, theta); }

void renderShape(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    drawRectangle(s->pos->pos, s->size, s->color);
    break;
  case CircleShape:
    drawCircle(s->pos->pos, s->radius, s->color);
    break;
  }
}

void setShapeVelocity(Shape *s, Pos *vel) {
  free(s->pos->vel);
  s->pos->vel = vel;
}
Pos *getShapeVelocity(Shape *s) { return s->pos->vel; }
Pos *getShapePosition(Shape *s) { return s->pos->pos; }
Pos *getShapeAcceleration(Shape *s) { return s->pos->acc; }

void setShapeAcceleration(Shape *s, Pos *acc) {
  free(s->pos->acc);
  s->pos->acc = acc;
}

void setShapePosition(Shape *s, Pos *pos) {
  free(s->pos->pos);
  s->pos->pos = pos;
}

typedef struct {
  Shape **shapes;
  int length;
} ShapeList;

ShapeList *newShapeList() {
  ShapeList *list = (ShapeList *)malloc(sizeof(ShapeList));
  list->length = 0;
  list->shapes = NULL;
  return list;
}

void addShapeToList(ShapeList *list, Shape *shape) {
  int i;
  Shape **newShapes = (Shape **)malloc((list->length + 1) * sizeof(Shape *));
  for (i = 0; i < list->length; i++)
    newShapes[i] = list->shapes[i];
  newShapes[list->length] = shape;
  free(list->shapes);
  list->shapes = newShapes;
}

void renderShapes(ShapeList *list) {
  int i;
  for (i = 0; i < list->length; i++) {
    renderShape(list->shapes[i]);
  }
}
void updateShapes(ShapeList *list, float theta) {
  int i;
  for (i = 0; i < list->length; i++) {
    updateShape(list->shapes[i], theta);
  }
}

float shapeTop(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->y + s->size->y / 2.0f;
  case CircleShape:
    return s->pos->pos->y + s->radius;
  }
  return 0;
}
float shapeBottom(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->y - s->size->y / 2.0f;
  case CircleShape:
    return s->pos->pos->y - s->radius;
  }
  return 0;
}

float shapeLeft(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->x - s->size->x / 2.0f;
  case CircleShape:
    return s->pos->pos->x - s->radius;
  }
  return 0;
}
float shapeRight(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->x + s->size->x / 2.0f;
  case CircleShape:
    return s->pos->pos->x + s->radius;
  }
  return 0;
}

void bounceShape(Shape *s, float x, float y) {
  if (x != 0) {
    s->pos->vel->x *= -x;
  }
  if (y != 0) {
    s->pos->vel->y *= -y;
  }
}

#endif // GAMA_SHAPE_INCLUDED
