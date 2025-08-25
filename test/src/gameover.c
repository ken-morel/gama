#include "defs.h"

Text *gameoverText;

void gameoverCreate(Scene *scene) {

  gameoverText = createTextNulled("Game over", playwrite, at(0, 0));
  gameoverText->color = RED;
  gameoverText->fontsize = 0.4;
}

void gameoverRender(Scene *scene) {
  renderText(gameoverText);
  //
}

Scene *creategameoverScene(App *app) {
  Scene *gameover = createScene(app);
  gameover->create = gameoverCreate;
  gameover->render = gameoverRender;
  gameover->background = BURLYWOOD;
  return gameover;
}
