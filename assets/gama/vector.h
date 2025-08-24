#ifndef GAMA_VECTOR_INCLUDED
#define GAMA_VECTOR_INCLUDED
#include <stdlib.h>

typedef struct {
  double x;
  double y;
} Pos;

Pos *at(double x, double y) {
  Pos *p = (Pos *)malloc(sizeof(Pos));
  p->x = x;
  p->y = y;
  return p;
}

typedef struct {
  Pos *pos;
  Pos *vel;
  Pos *acc;
} Vector;

Vector *createVector(Pos *pos, Pos *vel, Pos *acc) {
  Vector *v = (Vector *)malloc(sizeof(Vector));

  v->pos = pos;
  v->vel = vel;
  v->acc = acc;
  return v;
}

Vector *vectorAt(Pos *pos) { return createVector(pos, at(0, 0), at(0, 0)); }

void updateVector(Vector *v, double theta) {
  v->pos->x += (v->vel->x * theta) + (0.5 * v->acc->x * theta * theta);
  v->pos->y += (v->vel->y * theta) + (0.5 * v->acc->y * theta * theta);
  v->vel->x += v->acc->x * theta;
  v->vel->y += v->acc->y * theta;
}

#endif // GAMA_VECTOR_INCLUDED
