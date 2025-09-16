#ifndef GAMA_COLOR_INCLUDED
#define GAMA_COLOR_INCLUDED

#include <GL/gl.h>

typedef unsigned int Color;
#include "_colors.h"

float getColorRed(Color col) {
  unsigned short a1 = (col & 0xFF000000) >> 24;
  return (float)a1 / 255.0f;
}
float getColorGreen(Color col) {
  unsigned short a1 = (col & 0x00FF0000) >> 16;
  return (float)a1 / 255.0f;
}
float getColorBlue(Color col) {
  unsigned short a1 = (col & 0x0000FF00) >> 8;
  return (float)a1 / 255.0f;
}
float getColorAlpha(Color col) {
  unsigned short a1 = (col & 0x000000FF);
  return (float)a1 / 255.0f;
}
void setClearColor(Color col) {
  glClearColor(getColorRed(col), getColorGreen(col), getColorBlue(col),
               getColorAlpha(col));
}
void setGLColor(Color col) {

  glColor3f(getColorRed(col), getColorGreen(col), getColorBlue(col));
}
#endif // GAMA_COLOR_INCLUDED
