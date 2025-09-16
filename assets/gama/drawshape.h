#ifndef GAMA_DRAWSHAPE_INCLUDED
#define GAMA_DRAWSHAPE_INCLUDED

#include "color.h"
#include "math.h"
#include <GL/gl.h>
#include <math.h>

void drawRectangle(Pos *pos, Pos *size, Color color) {
  SetGLColor(color);
  double w = size->x / 2.0f;
  double h = size->y / 2.0f;
  glBegin(GL_QUADS);
  {
    glVertex2f(pos->x - w, pos->y - h); // top left
    glVertex2f(pos->x + w, pos->y - h); // top right
    glVertex2f(pos->x + w, pos->y + h); // bottom right
    glVertex2f(pos->x - w, pos->y + h); // bottom left
  }
  glEnd();
}
void drawCircle(Pos *pos, double radius, Color color) {
  double x, y;
  double step = 0.1;
  double angle = -step;
  SetGLColor(color);
  glBegin(GL_TRIANGLE_FAN);
  {
    glVertex2f(pos->x, pos->y);
    do {
      x = sin(angle) * radius;
      y = cos(angle) * radius;
      glVertex2f(pos->x + x, pos->y + y);

      angle += step;
    } while (angle < 2 * PI);
  }
  glEnd();
}

#endif
