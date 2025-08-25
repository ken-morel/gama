#include "defs.h"

Text *welcomeText;

void welcomeCreate(Scene *scene) {
  welcomeText = createTextNulled("Press p to play\n", ubuntu, at(0, 0));
  welcomeText->color = DARKBLUE;
}

void welcomeRender(Scene *scene) {
  renderText(welcomeText);
  //
}

void welcomeKey(Scene *scene, KeyEvent *e) {
  if (e->key == KeyP) {
    showScene(scene->app, robiScene);
  }
}

Scene *createWelcomeScene(App *app) {
  Scene *welcome = createScene(app);
  welcome->create = welcomeCreate;
  welcome->render = welcomeRender;
  welcome->background = CYAN;
  welcome->onkey = welcomeKey;
  return welcome;
}
