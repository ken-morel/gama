#ifndef GAMA_APP_INCLUDED
#define GAMA_APP_INCLUDED

#include "color.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
  float x;
  float y;
  int button;
} MouseClickEvent;

#include "_key.h"

typedef struct {
  Key key;
} KeyEvent;

typedef struct {
  char *title;
  unsigned int width;
  unsigned int height;
  void (*onclick)(MouseClickEvent *);
  void (*onkey)(KeyEvent *);
} App;

App *GamaCreateApp() {
  App *app = (App *)malloc(sizeof(App));
  app->title = "Test gama window";
  app->width = 600;
  app->height = 600;
  app->onclick = NULL;
  app->onkey = NULL;
  return app;
}

void SetAppTitle(App *app, char *title) {
  // FIX: Fix this
  // free(app->title);
  app->title = title;
}

#endif // GAMA_APP_INCLUDED
