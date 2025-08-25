#include "defs.h"

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  robiScene = createRobiScene(app);
  welcomeScene = createWelcomeScene(app);
  gameoverScene = creategameoverScene(app);
  if (robiScene == NULL || welcomeScene == NULL || gameoverScene == NULL) {
    puts("Failed to create scene");
    exit(1);
  }
}
void create(App *app) {
  ubuntu = loadFont("assets/fonts/Ubuntu-R.ttf");
  playwrite = loadFont("assets/fonts/PlaywriteHU-VariableFont_wght.ttf");
  if (ubuntu == NULL || playwrite == NULL) {
    printf("Error loading Ubuntu font");
    exit(11);
  }
  showScene(app, welcomeScene);
}
void shutdown(App *app) { puts("Good bye"); }
