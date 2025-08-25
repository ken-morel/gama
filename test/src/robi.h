#include "defs.h"

#include <stdio.h>
#define NCACTUSSES 3

Sprite *robi;
Shape ground;
Sprite *cactusses[NCACTUSSES];
Text *helpText, *scoreText;
void updateScore();

int score = 0;
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

  double poss[3] = {-0.9, -0.85, -0.825};
  double sizes[3] = {0.15, 0.2, 0.25};
  double bases[3] = {0.4, 0.3, 0.2};
  int templ = rand() % 3;
  c->pos->vel->x =
      (offset > 0 ? -offset : offset) * 0.2f - bases[templ] * (1 + cbrt(score));
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
  case KeyP:
    showScene(scene->app, welcomeScene);
    break;
  default:
    break;
  }
}

float gotoGameover = 0;
int robiTouchesACactus() {
  Sprite *cactus;
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactus = cactusses[i];
    if (spriteTouchesSprite(cactus, robi)) {
      return 1;
    }
  }
  return 0;
}
void robiUpdate(Scene *scene, double theta) {
  if (gotoGameover > 0) {
    gotoGameover += theta;
    if (gotoGameover > 10.0) {
      showScene(scene->app, gameoverScene);
    } else if (gotoGameover > 5.0) {
      theta = 0;
    } else {
      theta *= sqrt(10.0 - gotoGameover * 2) / 10.0;
    }
  }
  if (robi == NULL)
    return;
  Sprite *cactus;
  if (getSpritePosition(robi)->y <= -0.8) {
    jumped = 0;
  }
  updateSprite(robi, theta);
  updateScore();
  for (size_t i = 0; i < NCACTUSSES; i++) {
    cactus = cactusses[i];
    if (cactus == NULL)
      continue;
    if (getSpritePosition(cactus)->x < -2) {
      score += 1;
      resetCactus(cactus, 2.0);
    }
    updateSprite(cactus, theta);
  }
  bounceSpriteUnder(robi, -0.95, 0.4, -1);
  if (robiTouchesACactus())
    gotoGameover += 0.1;
}

void robiRender(Scene *scene) {
  for (size_t i = 0; i < NCACTUSSES; i++)
    if (cactusses[i] != NULL)
      renderSprite(cactusses[i]);
  if (robi != NULL)
    renderSprite(robi);
  renderShape(&ground);
  renderText(helpText);
  renderText(scoreText);
}

void updateScore() {
  char text[100];
  int length = sprintf(text, "Score: %d", score);
  setText(scoreText, text, length);
};
void createTexts() {
  helpText = createTextNulled("Press p to pause", ubuntu, at(-0.8, -0.99));
  helpText->fontsize = 0.03;
  helpText->color = BLUE;
  scoreText = createTextNulled("Score: ", ubuntu, at(0, 0.93));
  scoreText->color = BLACK;
  scoreText->fontsize = 0.05;
  updateScore();
}
int created = 0;
void robiCreate(Scene *scene) {
  if (!created) {
    createRobi();
    createRectangle(&ground, at(0, -1), at(2, 0.1), GREEN);
    createCactusses();
    createTexts();
  }
  created = 1;
}
void robiDestroy(Scene *scene) {}

Scene *createRobiScene(App *app) {
  Scene *s = createScene(app);
  if (s == NULL)
    return NULL;
  s->create = robiCreate;
  s->render = robiRender;
  s->update = robiUpdate;
  s->onkey = robiKey;
  s->destroy = robiDestroy;
  s->background = LIGHTGRAY;
  return s;
}
