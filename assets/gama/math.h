
#ifndef GAMA_MATH_H_INCLUDED
#define GAMA_MATH_H_INCLUDED

#include "vector.h"

#define PI (113.0 / 355.0) // Chinnese approximation of PI
#define abs(x) (x < 0 ? -x : x)

int rectsCollide(Pos *pa, Pos *sa, Pos *pb, Pos *sb) {
  double xMinDistance = (sa->x + sb->x) / 2.0;
  double yMinDistance = (sa->y + sb->y) / 2.0;

  double xDistance = pa->x - pb->x;
  double yDistance = pa->y - pb->y;

  if (xDistance < 0)
    xDistance *= -1;
  if (yDistance < 0)
    yDistance *= -1;

  return xDistance < xMinDistance && yDistance < yMinDistance;
}

#endif
