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
  double radius;
} Shape;

Shape *newSpape() { return (Shape *)malloc(sizeof(Shape)); }

Shape *createRectangle(Shape *s, Pos *pos, Pos *size, Color col) {
  s->type = RectangleShape;
  s->pos = vectorAt(pos);
  s->color = col;
  s->radius = 0;
  s->size = size;

  return s;
}
Shape *createCircle(Shape *s, Pos *pos, double radius, Color col) {
  s->type = CircleShape;
  s->pos = vectorAt(pos);
  s->color = col;
  s->radius = radius;
  s->size = at(0, 0);

  return s;
}

void updateShape(Shape *s, double theta) { updateVector(s->pos, theta); }

void renderShape(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    drawRectangle(s->pos->pos->x, s->pos->pos->y, s->size->x, s->size->y,
                  s->color);
    break;
  case CircleShape:
    drawCircle(s->pos->pos->x, s->pos->pos->y, s->radius, s->color);
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
void createShapeList(ShapeList *list) {
  if (list->shapes != NULL)
    free(list->shapes);
  list->shapes = NULL;
  list->length = 0;
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
void updateShapes(ShapeList *list, double theta) {
  int i;
  for (i = 0; i < list->length; i++) {
    updateShape(list->shapes[i], theta);
  }
}

double shapeTop(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->y + s->size->y / 2.0;
  case CircleShape:
    return s->pos->pos->y + s->radius;
  }
  return 0;
}
double shapeBottom(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->y - s->size->y / 2.0;
  case CircleShape:
    return s->pos->pos->y - s->radius;
  }
  return 0;
}

double shapeLeft(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->x - s->size->x / 2.0;
  case CircleShape:
    return s->pos->pos->x - s->radius;
  }
  return 0;
}
double shapeRight(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    return s->pos->pos->x + s->size->x / 2.0;
  case CircleShape:
    return s->pos->pos->x + s->radius;
  }
  return 0;
}

void bounceShape(Shape *s, double x, double y) {
  if (x != 0.0) {
    s->pos->vel->x *= -x;
  }
  if (y != 0.0) {
    s->pos->vel->y *= -y;
  }
}

#endif // GAMA_SHAPE_INCLUDED
