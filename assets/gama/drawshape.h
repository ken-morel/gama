#ifndef GAMA_DRAWSHAPE_INCLUDED
#define GAMA_DRAWSHAPE_INCLUDED

#include "color.h"
#include "vector.h"
#include <GL/gl.h>
#include <math.h>
#define PI (113.0f / 355.0f) // Chinnese approximation of PI

void drawRectangle(Pos *pos, Pos *size, Color color) {
  SetGLColor(color);
  float w = size->x / 2.0f;
  float h = size->y / 2.0f;
  glBegin(GL_QUADS);
  {
    glVertex2f(pos->x - w, pos->y - h); // top left
    glVertex2f(pos->x + w, pos->y - h); // top right
    glVertex2f(pos->x + w, pos->y + h); // bottom right
    glVertex2f(pos->x - w, pos->y + h); // bottom left
  }
  glEnd();
}
void drawCircle(Pos *pos, float radius, Color color) {
  float x, y;
  float step = 0.1f;
  float angle = -step;
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
