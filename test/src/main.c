#include "../../assets/gama/gama.h"

Scene *robiScene, *welcomeScene;
Font *ubuntu;
#include "robi.h"
#include "welcome.h"

void init(App *app) {
  srand(time(NULL));
  SetAppTitle(app, "floating Robi game");
  robiScene = createRobiScene(app);

  welcomeScene = createWelcomeScene(app);
  if (robiScene == NULL || welcomeScene == NULL) {
    puts("Failed to create scene");
    exit(1);
  }
  ubuntu = loadFont("assets/fonts/Ubuntu-R.ttf");
  if (ubuntu == NULL) {
    printf("Error loading Ubuntu font");
    exit(11);
  }
}
void create(App *app) { showScene(app, welcomeScene); }
void shutdown(App *app) { puts("Good bye"); }
