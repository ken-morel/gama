#include "../../../assets/gama/gama.h"

void welcomeCreate(Scene *scene) {}

void welcomeRender(Scene *scene) {}

void test(Scene *scene, MouseClickEvent *e) {
  if (!e->down)
    showScene(scene->app, robiScene);
}

Scene *createWelcomeScene(App *app) {
  Scene *welcome = createScene(app);
  welcome->create = welcomeCreate;
  welcome->render = welcomeRender;
  welcome->background = CYAN;
  welcome->onclick = test;
  return welcome;
}
