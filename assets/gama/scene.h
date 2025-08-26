#ifndef GAMA_SCENE_INCLUDED
#define GAMA_SCENE_INCLUDED

#include "color.h"
#include "events.h"
#include "object.h"
#include "shape.h"
#include "sprite.h"
#include "text.h"
#include <cstddef>

struct sApp;

typedef struct sScene {
  Color background;
  GObjectList objects;
  struct sApp *app;
  void (*create)(struct sScene *);
  void (*update)(struct sScene *, double);
  void (*render)(struct sScene *);
  void (*destroy)(struct sScene *);

  void (*onclick)(struct sScene *, MouseClickEvent *);
  void (*onkey)(struct sScene *, KeyEvent *);

} Scene;

Scene *createScene(struct sApp *app) {
  Scene *s = (Scene *)malloc(sizeof(Scene));
  if (s == NULL)
    return NULL;
  s->app = app;
  s->create = NULL;
  s->update = NULL;
  s->render = NULL;
  s->destroy = NULL;
  s->onclick = NULL;
  s->onkey = NULL;
  s->background = LIGHTCYAN;
  createGObjectList(&s->objects);
  return s;
}

void addObjectToScene(Scene *s, GObject *o) {
  addGObjectToList(&s->objects, o);
}
GObject *popObjectFromScene(Scene *s) {
  return popGObjectFromList(&s->objects);
}

void sceneCreate(Scene *s) {
  if (s->create != NULL)
    s->create(s);
}
void sceneUpdate(Scene *s, double theta) {
  for (int i = 0; i < s->objects.length; i++)
    updateGObject(s->objects.objects[i], theta);

  if (s->update != NULL)
    s->update(s, theta);
}
void sceneRender(Scene *s) {
  SetClearColor(s->background);

  for (int i = 0; i < s->objects.length; i++)
    renderGObject(s->objects.objects[i]);

  if (s->render != NULL)
    s->render(s);
}
void sceneDestroy(Scene *s) {
  if (s->destroy != NULL)
    s->destroy(s);
}

void sceneClick(Scene *s, MouseClickEvent *e) {
  if (s->onclick != NULL)
    s->onclick(s, e);
}
void sceneKey(Scene *s, KeyEvent *e) {
  if (s->onkey != NULL)
    s->onkey(s, e);
}

#endif
