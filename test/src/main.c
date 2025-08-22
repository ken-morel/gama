#include "../../assets/gama/gama.h"

Shape *rectangle;

int init(App *app) {
  app->title = "My gama window";
  app->size.x = 600;
  app->size.y = 600;
  return 0;
}

int create() {
  rectangle = createRectangle(at(0, 0), at(1, 1), GREEN);
  SetClearColor(LIGHTBLUE);
  return 0;
}

int update(float theta) {
  updateShape(rectangle, theta);
  return 0;
}

int render() {
  // glRotatef(1, 0, 1, 1);
  renderShape(rectangle);
  return 0;
}

int shutdown() {
  free(rectangle);
  return 0;
}
