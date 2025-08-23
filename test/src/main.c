#include "../../assets/gama/gama.h"

Shape *ball, *earth;

int init(App *app) {
  SetAppTitle(app, "floating");
  return 0;
}

int create() {
  ball = createRectangle(at(0, 1), at(1, 1), RED);
  earth = createRectangle(at(0, -1), at(2, 0.1), DARKGREEN);

  setShapeAcceleration(ball, at(0, -0.9));

  SetClearColor(LIGHTGRAY);
  return 0;
}

int update(float theta) {
  updateShape(ball, theta);
  if (shapeBottom(ball) < shapeTop(earth)) {
    if (getShapeVelocity(ball)->y < 0) { // shape goes down
      bounceShape(ball, 0, 0.8);
    }
  }
  return 0;
}

int render() {
  renderShape(ball);
  renderShape(earth);
  return 0;
}

int shutdown() { return 0; }
