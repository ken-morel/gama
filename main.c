#include "gama.h"

Shape2d ground, ball;

void init() {
    ground = CreateRectangle(0, -0.9, 2, 0.1);
    ground->color = DARKGREEN;

    ball = CreateCircle(0, 1, 0.1);
    ball->color = YELLOW;
    ball->pos->a->y = -0.981;
}

void update(float theta) {
    if(G2ShapeBottom(ball) <= G2ShapeTop(ground) && G2ShapeFalling(ball))
        G2BounceY(ball, 0.1);
    G2Update(ball, theta);
}

void render() {
    G2Render(ground);
    G2Render(ball);
}
