#ifndef GAMA_INCLUDED
#define GAMA_INCLUDED

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include "app.h"
#include "scene.h"

#ifdef BACKEND_GLFW
#include "backends/glfw.h"

#endif // BACKEND_GLFW

#ifdef BACKEND_WIN32
#include "backends/win32.h"
#endif // Backend win32

#ifdef BACKEND_EMSCRIPTEN
#include "backends/emscripten.h"
#endif // BACKEND_EMSCRIPTEN

#include "gtime.h"

void gama_click(App *app, MouseClickEvent *e) {
  if (app->onclick != NULL)
    app->onclick(app, e);
  if (app->scene != NULL) {
    sceneClick(app->scene, e);
  }
}
void gama_key(App *app, KeyEvent *e) {
  if (app->onkey != NULL)
    app->onkey(app, e);
  if (app->scene != NULL) {
    sceneKey(app->scene, e);
  }
}

double lastTime = 0.0;

void init(App *);
void create(App *);
void shutdown(App *);

void _gama_init(App *app) {
  lastTime = get_elapsed_seconds();
  init(app);
}
void _gama_create(App *app) { create(app); }
void _gama_update(App *app) {
  double now = get_elapsed_seconds();
  double theta = now - lastTime;
  if (app->scene != NULL)
    sceneUpdate(app->scene, theta);
  lastTime = now;
}
void _gama_render(App *app) {
  if (app->scene != NULL)
    sceneRender(app->scene);
}
void _gama_shutdown(App *app) {
  if (app->scene != NULL && app->scene->destroy != NULL)
    sceneDestroy(app->scene);
  shutdown(app);
}

#endif // GAMA_INCLUDED
