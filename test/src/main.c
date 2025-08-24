#include "robi/robi.h"

Scene *robiScene;

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  robiScene = createRobiScene(app);
  if (robiScene == NULL) {
    puts("Failed to create robi scene");
    exit(1);
  }
}
void create(App *app) { showScene(app, robiScene); }
void shutdown(App *app) { puts("Good bye"); }
