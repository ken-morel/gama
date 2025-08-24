#include "../../assets/gama/gama.h"

Text *welcomeText;

void welcomeCreate(Scene *scene) {

  // welcomeText = createTextNulled("Press p to play\n", ubuntu, at(0, 0));
  welcomeText = createText("Hello", 5, ubuntu, at(0, 0));
  welcomeText->fontsize = 0.05;
  // welcomeText->fontsize = 0.1;
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
