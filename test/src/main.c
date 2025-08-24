#include "game.h"

Scene *robiScene;

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  robiScene = createRobiScene(app);
}
void create(App *app) { showScene(app, robiScene); }
void shutdown(App *app) { puts("Good bye"); }
