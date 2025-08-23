#include "../../assets/gama/gama.h"

Sprite *robi;
Shape *ground;
int jumped = 0;

Sprite *createRobi() {
  Sprite *robi =
      openSprite("assets/sprites/robi.png", 20, 20, at(-0.8, 0), at(0.2, 0.2));
  if (robi == NULL) {
    exit(5);
  }
  unsigned int animation[] = {0, 1, 2, 3, 2, 1};
  SetSpriteAnimationArray(robi, animation, 10);
  setSpriteAcceleration(robi, at(0, -1.0));
  return robi;
}

Shape *createGround() { return createRectangle(at(0, -1), at(2, 0.1), GREEN); }

void jump() {
  setSpriteVelocity(robi, at(0, 1));

  //
}

void onkey(KeyEvent *e) {
  switch (e->key) {
  case KeySpace:
  case KeyUp:
  case KeyW:
    if (jumped < 2) {
      jump();
      jumped++;
    }
    break;
  default:
    break;
  }
}
void onclick(MouseClickEvent *e) { printf("clicked\n"); }

void init(App *app) {
  SetAppTitle(app, "floating Robi game");
  app->onkey = onkey;
}

void create() {
  robi = createRobi();
  ground = createGround();
  SetClearColor(LIGHTGRAY);
}

void update(float theta) {

  if (getSpritePosition(robi)->y <= -0.8) {
    jumped = 0;
  }
  updateSprite(robi, theta);
  bounceSpriteUnder(robi, -0.95, 0.4, -1);
}

void render() {
  renderSprite(robi);
  renderShape(ground);
}
