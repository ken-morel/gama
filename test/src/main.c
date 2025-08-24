#include "../../assets/gama/gama.h"

Scene *robiScene, *welcomeScene;
#include "robi/robi.h"
#include "welcome/welcome.h"

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  robiScene = createRobiScene(app);

  welcomeScene = createWelcomeScene(app);
  if (robiScene == NULL || welcomeScene == NULL) {
    puts("Failed to create scene");
    exit(1);
  }
}
void create(App *app) { showScene(app, welcomeScene); }
void shutdown(App *app) { puts("Good bye"); }
