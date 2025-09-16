#ifndef GAMA_VECTOR_INCLUDED
#define GAMA_VECTOR_INCLUDED

#include <string.h>
#define VectorAt(pos) createVector(pos, at(0, 0), at(0, 0))

typedef struct {
  double x;
  double y;
} Pos;

Pos at(double x, double y) {
  Pos p = {
      .x = x,
      .y = y,
  };
  return p;
}

int resetPos(Pos *p) {
  if (p == NULL)
    return 0;
  p->x = 0;
  p->y = 0;
  return 1;
}
int initPos(Pos *p, double x, double y) {
  if (p == NULL)
    return 0;
  p->x = x;
  p->y = y;
  return 1;
}
int copyPos(Pos *from, Pos *to) {
  if (from == NULL || to == NULL)
    return 0;
  to->x = from->x;
  to->y = from->y;
  return 1;
}

typedef struct {
  double x;
  double y;
  Pos vel;
  Pos acc;
} Vector;

Pos vectorPos(Vector *v) {
  if (v == NULL)
    return at(0, 0);
  return at(v->x, v->y);
}

Vector createVector(Pos pos, Pos vel, Pos acc) {
  Vector v = {
      .x = pos.x,
      .y = pos.y,
      .vel = vel,
      .acc = acc,
  };
  return v;
}
int initVector(Vector *v, Pos p, Pos vel, Pos acc) {
  if (v == NULL)
    return 0;
  v->x = p.x;
  v->y = p.y;
  copyPos(&vel, &v->vel);
  copyPos(&acc, &v->acc);
  return 1;
}
int copyVector(Vector *from, Vector *to) {
  if (from == NULL || to == NULL)
    return 0;
  from->x = to->x;
  from->y = to->y;
  copyPos(&from->vel, &to->vel);
  copyPos(&from->acc, &to->acc);
  return 1;
}
void resetVector(Vector *v) {
  v->x = 0;
  v->y = 0;
  resetPos(&v->acc);
  resetPos(&v->vel);
}

void updateVector(Vector *v, double theta) {
  v->x += (v->vel.x * theta) + (0.5 * v->acc.x * theta * theta);
  v->y += (v->vel.y * theta) + (0.5 * v->acc.y * theta * theta);
  v->vel.x += v->acc.x * theta;
  v->vel.y += v->acc.y * theta;
}

#endif // GAMA_VECTOR_INCLUDED
