#include "../../../assets/gama/gama.h"

Font *ubuntu;
Text *welcomeText;

void welcomeCreate(Scene *scene) {
  printf("loading font");
  ubuntu = loadFont("assets/fonts/Ubuntu-R.ttf");
  if (ubuntu == NULL) {
    printf("Error loading Ubuntu font");
    exit(11);
  }
  welcomeText = createText("Welcome!", 8, ubuntu, at(0, 0));
  if (welcomeText == NULL) {
    printf("Error creating text");
    exit(12);
  }
}

void welcomeRender(Scene *scene) {
  renderText(welcomeText);
  //
}

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
