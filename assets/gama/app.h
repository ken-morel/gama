#ifndef GAMA_APP_INCLUDED
#define GAMA_APP_INCLUDED

#include "color.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
  char *title;
  Pos size;
} App;

App *GamaCreateApp() {
  App *app = (App *)malloc(sizeof(App));
  app->title = "Test gama window";
  app->size.x = 600;
  app->size.y = 400;
  return app;
}

void SetAppTitle(App *app, char *title) {
  free(app->title);
  app->title = title;
}

#endif // GAMA_APP_INCLUDED
