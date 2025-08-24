#include "../../../assets/gama/gama.h"
#include <stdio.h>
#define NCACTUSSES 3

Sprite *robi;
Shape ground;
Sprite *cactusses[NCACTUSSES];

int jumped = 0;

void createRobi() {
  robi =
      createSprite("assets/sprites/robi.png", 20, 20, at(-0.8, 0), at(0.2, 0));
  if (robi == NULL)
    exit(1);
  unsigned int animation[] = {0, 1, 2, 3, 2, 1};
  SetSpriteAnimationArray(robi, animation, 10);
  setSpriteAcceleration(robi, at(0, -2.0));
}

void resetCactus(Sprite *c, float around) {
  // random float between -1 and 1
  float offset = ((float)(rand() % 2000) / 1000.0f) - 1.0f;
  if (offset > 1)
    offset = 1;
  else if (offset < -1)
    offset = -1;
  c->pos->pos->x = around + (1.0f * offset);
  c->pos->vel->x = (offset > 0 ? -offset : offset) * 0.1f - 0.3f;

  double poss[3] = {-0.9, -0.85, -0.825};
  double sizes[3] = {0.15, 0.2, 0.25};
  int templ = rand() % 3;
  c->pos->pos->y = poss[templ];
  c->size->y = sizes[templ];
}
void createCactusses() {
  unsigned int anim[] = {0, 1};
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactusses[i] =
        createSprite("assets/sprites/cactus.png", 20, 30, at(0, 0), at(0, 0));
    if (cactusses[i] == NULL)
      exit(3);
    resetCactus(cactusses[i], 2.0 + 1.0 * (double)i);
    SetSpriteAnimationArray(cactusses[i], anim, 2);
  }
}

void robiKey(Scene *scene, KeyEvent *e) {
  switch (e->key) {
  case KeySpace:
  case KeyUp:
  case KeyW:
  case KeyEnter:
    if (jumped < 2) {
      setSpriteVelocity(robi, at(0, 1));
      jumped++;
    }
    break;
  default:
    break;
  }
}

void robiUpdate(Scene *scene, double theta) {
  if (robi == NULL)
    return;
  Sprite *cactus;
  if (getSpritePosition(robi)->y <= -0.8) {
    jumped = 0;
  }
  updateSprite(robi, theta);
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactus = cactusses[i];
    if (cactus == NULL)
      continue;
    if (getSpritePosition(cactus)->x < -2) {
      resetCactus(cactus, 2.0);
    }
    updateSprite(cactus, theta);
  }
  bounceSpriteUnder(robi, -0.95, 0.4, -1);
}

void robiRender(Scene *scene) {
  for (size_t i = 0; i < NCACTUSSES; i++)
    if (cactusses[i] != NULL)
      renderSprite(cactusses[i]);
  if (robi != NULL)
    renderSprite(robi);
  renderShape(&ground);
}

int created = 0;
void robiCreate(Scene *scene) {
  if (!created) {
    createRobi();
    createRectangle(&ground, at(0, -1), at(2, 0.1), GREEN);
    createCactusses();
  }
  created = 1;
}
void robiDestroy(Scene *scene) {}

void test2(Scene *scene, MouseClickEvent *e) {
  if (!e->down)
    showScene(scene->app, welcomeScene);
}

Scene *createRobiScene(App *app) {
  Scene *s = createScene(app);
  if (s == NULL)
    return NULL;
  s->create = robiCreate;
  s->render = robiRender;
  s->update = robiUpdate;
  s->onkey = robiKey;
  s->onclick = test2;
  s->destroy = robiDestroy;
  s->background = LIGHTGRAY;
  return s;
}
