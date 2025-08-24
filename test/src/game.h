

#include "../../assets/gama/gama.h"
#define NCACTUSSES 3

Sprite robi;
Shape ground;
Sprite cactusses[NCACTUSSES];

int jumped = 0;

void createRobi() {
  createSprite(&robi, "assets/sprites/robi.png", 20, 20, at(-0.8, 0),
               at(0.2, 0));
  unsigned int animation[] = {0, 1, 2, 3, 2, 1};
  SetSpriteAnimationArray(&robi, animation, 10);
  setSpriteAcceleration(&robi, at(0, -2.0));
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
  Sprite *cactus;
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactus = &cactusses[i];
    createSprite(cactus, "assets/sprites/cactus.png", 20, 30, at(0, 0),
                 at(0, 0));
    resetCactus(cactus, 2.0 + 1.0 * (double)i);

    if (cactus == NULL)
      exit(3);
    SetSpriteAnimationArray(cactus, anim, 2);
  }
}

void robiKey(Scene *scene, KeyEvent *e) {
  switch (e->key) {
  case KeySpace:
  case KeyUp:
  case KeyW:
  case KeyEnter:
    if (jumped < 2) {
      setSpriteVelocity(&robi, at(0, 1));
      jumped++;
    }
    break;
  default:
    break;
  }
}

void robiUpdate(Scene *scene, double theta) {
  Sprite *cactus;
  if (getSpritePosition(&robi)->y <= -0.8) {
    jumped = 0;
  }
  updateSprite(&robi, theta);
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactus = &cactusses[i];
    if (getSpritePosition(cactus)->x < -2) {
      resetCactus(cactus, 2.0);
    }
    updateSprite(cactus, theta);
  }
  bounceSpriteUnder(&robi, -0.95, 0.4, -1);
}

void robiRender(Scene *scene) {
  for (size_t i = 0; i < NCACTUSSES; i++)
    renderSprite(&cactusses[i]);
  renderSprite(&robi);
  renderShape(&ground);
}

int created = 0;
void robiCreate(Scene *scene) {
  if (!created) {
    createRobi();
    createRectangle(&ground, at(0, -1), at(2, 0.1), GREEN);
    createCactusses();
  }
  SetClearColor(LIGHTGRAY);
  created = 1;
}
void robiDestroy(Scene *scene) {}

Scene *createRobiScene(App *app) {
  Scene *s = createScene(app);
  s->create = robiCreate;
  s->render = robiRender;
  s->update = robiUpdate;
  s->onkey = robiKey;
  s->destroy = robiDestroy;
}
