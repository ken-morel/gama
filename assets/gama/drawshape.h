#ifndef GAMA_DRAWSHAPE_INCLUDED
#define GAMA_DRAWSHAPE_INCLUDED

#include "color.h"
#include "vector.h"
#include <GL/gl.h>
#include <math.h>
#define PI (113.0f / 355.0f) // Chinnese approximation of PI

void drawRectangle(double x, double y, double width, double height,
                   Color color) {
  SetGLColor(color);
  double w = width / 2.0f;
  double h = height / 2.0f;
  glBegin(GL_QUADS);
  {
    glVertex2f(x - w, y - h); // top left
    glVertex2f(x + w, y - h); // top right
    glVertex2f(x + w, y + h); // bottom right
    glVertex2f(x - w, y + h); // bottom left
  }
  glEnd();
}
void drawCircle(double cx, double cy, double radius, Color color) {
  double x, y;
  double step = 0.1;
  double angle = -step;
  SetGLColor(color);
  glBegin(GL_TRIANGLE_FAN);
  {
    glVertex2f(cx, cy);
    do {
      x = sin(angle) * radius;
      y = cos(angle) * radius;
      glVertex2f(cx + x, cy + y);

      angle += step;
    } while (angle < 2 * PI);
  }
  glEnd();
}

#endif
