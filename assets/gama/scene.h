#ifndef GAMA_SCENE_INCLUDED
#define GAMA_SCENE_INCLUDED

#include "events.h"
#include "shape.h"
#include "sprite.h"

struct sApp;

typedef struct sScene {
  SpriteList sprites;
  ShapeList shapes;
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
  s->sprites.sprites = NULL;
  s->shapes.shapes = NULL;
  createSpriteList(&s->sprites);
  createShapeList(&s->shapes);
  s->app = app;
  s->create = NULL;
  s->update = NULL;
  s->render = NULL;
  s->destroy = NULL;
  s->onclick = NULL;
  s->onkey = NULL;
  return s;
}

void addShapeToScene(Scene *scene, Shape *shape) {
  addShapeToList(&scene->shapes, shape);
}
void addSpriteToScene(Scene *scene, Sprite *sprite) {
  addSpriteToList(&scene->sprites, sprite);
}

void sceneCreate(Scene *s) {
  if (s->create != NULL)
    s->create(s);
}
void sceneUpdate(Scene *s, double theta) {
  if (s->update != NULL)
    s->update(s, theta);
}
void sceneRender(Scene *s) {
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
