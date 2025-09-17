#ifndef GAMA_SHAPE_INCLUDED
#define GAMA_SHAPE_INCLUDED

#include "color.h"
#include "vector.h"

#include "drawshape.h"
#include <malloc.h>

typedef enum {
  NoShape,
  RectangleShape,
  CircleShape,
} ShapeType;

typedef struct {
  ShapeType type;
  Vector pos;
  Pos size;
  Color color;
  double radius;
} Shape;

Shape *newShape() {
  Shape *s = (Shape *)malloc(sizeof(Shape));
  s->color = BLACK;
  resetVector(&s->pos);
  s->radius = 0;
  resetPos(&s->size);
  s->type = CircleShape;
  return s;
}
int resetShape(Shape *s) {
  if (s == NULL)
    return 0;
  s->color = BLACK;
  resetVector(&s->pos);
  resetPos(&s->size);
  s->radius = 0;
  s->type = NoShape;
  return 1;
}

int initRectangle(Shape *s, Pos pos, Pos size, Color col) {
  if (!resetShape(s)) // does NULL check
    return 0;

  s->type = RectangleShape;
  s->pos = VectorAt(pos);
  s->size = size;
  s->color = col;
  return 1;
}
int initCircle(Shape *s, Pos pos, double radius, Color col) {
  if (!resetShape(s)) // does NULL check
    return 0;

  s->type = CircleShape;
  s->pos = VectorAt(pos);
  s->color = col;
  s->radius = radius;
  return 1;
}

void updateShape(Shape *s, double theta) { updateVector(&s->pos, theta); }

int renderShape(Shape *s) {
  if (!s)
    return 0;

  Pos pos = {s->pos.x, s->pos.y};
  switch (s->type) {
  case RectangleShape:
    drawRectangle(&pos, &s->size, s->color);
    return 1;
  case CircleShape:
    drawCircle(&pos, s->radius, s->color);
    return 1;
  case NoShape:
    return 0;
  }
}

Pos getShapeVelocity(Shape *s) { return s ? s->pos.vel : at(0, 0); }
Pos getShapePosition(Shape *s) { return s ? vectorPos(&s->pos) : at(0, 0); }
Pos getShapeAcceleration(Shape *s) { return s ? s->pos.acc : at(0, 0); }

int setShapeVelocity(Shape *s, Pos *vel) {
  if (!s || !vel)
    return 0;
  copyPos(vel, &s->pos.vel);
  return 1;
}
int setShapeAcceleration(Shape *s, Pos *acc) {
  if (!s || !acc)
    return 0;
  copyPos(acc, &s->pos.acc);
  return 1;
}

int setShapePosition(Shape *s, Pos *pos) {
  if (!s)
    return 0;
  s->pos.x = pos->x;
  s->pos.y = pos->y;
  return 1;
}

double shapeTopY(Shape *s) {
  if (!s)
    return 0;
  switch (s->type) {
  case RectangleShape:
    return s->pos.y + s->size.y / 2.0;
  case CircleShape:
    return s->pos.y + s->radius;
  case NoShape:
    return 0;
  }
}
double shapeBottomY(Shape *s) {
  if (!s)
    return 0;
  switch (s->type) {
  case RectangleShape:
    return s->pos.y - s->size.y / 2.0;
  case CircleShape:
    return s->pos.y - s->radius;
  case NoShape:
    return 0;
  }
}

double shapeLeftX(Shape *s) {
  if (!s)
    return 0;
  switch (s->type) {
  case RectangleShape:
    return s->pos.x - s->size.x / 2.0;
  case CircleShape:
    return s->pos.x - s->radius;
  case NoShape:
    return 0;
  }
}
double shapeRightX(Shape *s) {
  if (!s)
    return 0;
  switch (s->type) {
  case RectangleShape:
    return s->pos.x + s->size.x / 2.0;
  case CircleShape:
    return s->pos.x + s->radius;
  case NoShape:
    return 0;
  }
}

void bounceShape(Shape *s, double x, double y) {
  if (x != 0.0)
    s->pos.vel.x *= -x;
  if (y != 0.0)
    s->pos.vel.y *= -y;
}

#endif // GAMA_SHAPE_INCLUDED
