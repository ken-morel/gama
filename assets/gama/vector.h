#ifndef GAMA_VECTOR_INCLUDED
#define GAMA_VECTOR_INCLUDED
#include <stdlib.h>

typedef struct {
  float x;
  float y;
} Pos;

Pos *at(float x, float y) {
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

void updateVector(Vector *v, float theta) {
  v->pos->x += (v->vel->x * theta) + (0.5f * v->acc->x * theta * theta);
  v->pos->y += (v->vel->y * theta) + (0.5f * v->acc->y * theta * theta);
  v->vel->x += v->acc->x * theta;
  v->vel->y += v->acc->y * theta;
}

#endif // GAMA_VECTOR_INCLUDED
