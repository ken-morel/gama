#ifndef GAMA_SHAPE_INCLUDED
#define GAMA_SHAPE_INCLUDED

#include "color.h"
#include "vector.h"

#include "drawshape.h"

typedef enum {
  RectangleShape,
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

void updateShape(Shape *s, float theta) { updateVector(s->pos, theta); }

void renderShape(Shape *s) {
  switch (s->type) {
  case RectangleShape:
    drawRectangle(s->pos->pos, s->size, s->color);
  }
}

void setShapeVelocity(Shape *s, Pos *vel) {
  free(s->pos->vel);
  s->pos->vel = vel;
}

void setShapeAcceleration(Shape *s, Pos *acc) {
  free(s->pos->acc);
  s->pos->acc = acc;
}

void setShapePosition(Shape *s, Pos *pos) {
  free(s->pos->pos);
  s->pos->pos = pos;
}

#endif // GAMA_SHAPE_INCLUDED
