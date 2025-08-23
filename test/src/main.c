#include "../../assets/gama/gama.h"
#define NCACTUSSES 5

void onkey(KeyEvent *e);
Sprite *robi;
Shape *ground;
Sprite *cactusses[NCACTUSSES];

int jumped = 0;

Sprite *createRobi() {
  Sprite *robi =
      openSprite("assets/sprites/robi.png", 20, 20, at(-0.8, 0), at(0.2, 0));
  if (robi == NULL) {
    exit(5);
  }
  unsigned int animation[] = {0, 1, 2, 3, 2, 1};
  SetSpriteAnimationArray(robi, animation, 10);
  setSpriteAcceleration(robi, at(0, -1.0));
  return robi;
}
Sprite *createCactus() {
  unsigned int anim[] = {0, 1};
  Sprite *cactus =
      openSprite("assets/sprites/cactus.png", 20, 30, at(0, 0), at(0.2, 0.2));

  if (cactus == NULL)
    exit(3);
  SetSpriteAnimationArray(cactus, anim, 2);
  return cactus;
}

Shape *createGround() { return createRectangle(at(0, -1), at(2, 0.1), GREEN); }

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  app->onkey = onkey;
}

void resetCactus(Sprite *c, float around) {
  return;
  // random float between -1 and 1
  float offset = ((float)(rand() % 2000) / 1000.0f) - 1000.0f;
  c->pos->pos->x = 0; // around + (1.0f * offset);
  c->pos->pos->y = -0.8;
  //                value between 0.0f and 0.1f           | base speed
  c->pos->vel->x = (offset < 0 ? -offset : offset) * -0.1f - 0.2f;
}

void create() {
  robi = createRobi();
  ground = createGround();
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactusses[i] = createCactus();
    resetCactus(cactusses[i], 2.0f + 1.0f * (float)i);
  }
  SetClearColor(LIGHTGRAY);
}
void jump() {
  setSpriteVelocity(robi, at(0, 1));

  //
}

void onkey(KeyEvent *e) {
  switch (e->key) {
  case KeySpace:
  case KeyUp:
  case KeyW:
  case KeyEnter:
    if (jumped < 2) {
      jump();
      jumped++;
    }
    break;
  default:
    break;
  }
}

void update(float theta) {

  if (getSpritePosition(robi)->y <= -0.8) {
    jumped = 0;
  }
  updateSprite(robi, theta);
  for (size_t i = 0; i < NCACTUSSES; i++)
    updateSprite(cactusses[i], theta);
  bounceSpriteUnder(robi, -0.95, 0.4, -1);
}

void render() {
  renderSprite(robi);
  renderShape(ground);
  for (size_t i = 0; i < NCACTUSSES; i++)
    renderSprite(cactusses[i]);
}
