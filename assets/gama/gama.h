
#ifndef GAMA_INCLUDED
#define GAMA_INCLUDED

#include "color.h"

#include "app.h"
#include "image.h"
#include "shape.h"
#include "sprite.h"

#include <stdio.h>
#include <time.h>

///////////////////////
#ifndef BACKEND_GLFW
#ifndef BACKEND_WIN32

#ifdef __linux__
#define BACKEND_GLFW
#endif
#ifdef __windows__
#define BACKEND_GLFW
#endif

#endif
#endif
////////////////////////

#ifdef BACKEND_GLFW
#include "backends/glfw.h"

#endif // BACKEND_GLFW

#ifdef BACKEND_WIN32
#include "backends/win32.h"
#endif // Backend win32
//
void gama_click(App *app, MouseClickEvent *e) {
  if (app->onclick != NULL) {
    app->onclick(e);
  }
}
void gama_key(App *app, KeyEvent *e) {
  if (app->onkey != NULL) {
    app->onkey(e);
  }
}

float lastTime = 0.0f;

double getT() { return (double)clock() / CLOCKS_PER_SEC; }

#error "convert all floats to doubles"

void init(App *);
void create(App *);
void update(App *, float);
void render(App *);

void _gama_init(App *app) {
  lastTime = getT();
  init(app);
}
void _gama_create(App *app) { create(app); }
void _gama_update(App *app) {
  float now = getT();
  update(app, now - lastTime);
  lastTime = now;
}
void _gama_render(App *app) { render(app); }
void _gama_shutdown(App *app) {}

#endif // GAMA_INCLUDED
