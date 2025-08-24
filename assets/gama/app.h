#ifndef GAMA_APP_INCLUDED
#define GAMA_APP_INCLUDED

#include "color.h"
#include "events.h"
#include "scene.h"
#include "vector.h"
#include <stdlib.h>

typedef struct sApp {
  char *title;
  unsigned int width;
  unsigned int height;
  void (*onclick)(struct sApp *, MouseClickEvent *);
  void (*onkey)(struct sApp *, KeyEvent *);
  Scene *scene;
} App;

App *GamaCreateApp() {
  App *app = (App *)malloc(sizeof(App));
  app->title = "Test gama window";
  app->width = 600;
  app->height = 600;
  app->onclick = NULL;
  app->onkey = NULL;
  app->scene = NULL;
  return app;
}

void showScene(App *app, Scene *s) {
  if (app->scene != NULL)
    sceneDestroy(app->scene);
  app->scene = s;
  sceneCreate(app->scene);
}

void SetAppTitle(App *app, char *title) {
  // FIX: Fix this
  // free(app->title);
  app->title = title;
}

void _gama_init(App *);
void _gama_create(App *);
void _gama_update(App *);
void _gama_render(App *);
void _gama_shutdown(App *);
void gama_click(App *, MouseClickEvent *);
void gama_key(App *, KeyEvent *);
App *GamaCreateApp();

#endif // GAMA_APP_INCLUDED
