#ifndef GAMA_DRAWSHAPE_INCLUDED
#define GAMA_DRAWSHAPE_INCLUDED

#include "color.h"
#include "vector.h"
#include <GL/gl.h>
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

#endif
