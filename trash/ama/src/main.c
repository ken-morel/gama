#include "../../../assets/gama/gama.h"

GObject shape = GN;

void sCreate(Scene *s) { shape = GRectangle(at(0, 0), at(1, 1), BLUE); }
void sRender(Scene *s) { Grender(shape); }
Scene *testScene(App *app) {
  Scene *s = createScene(app);
  s->create = sCreate;
  s->render = sRender;
  return s;
}
// Initialize the applicaiton here
void init(App *app) {}

// Create scenes, shapes, and bind them to the app
void create(App *app) {
  Scene *s = testScene(app);
  showScene(app, s);
}

// When app closes
void shutdown(App *app) {}
