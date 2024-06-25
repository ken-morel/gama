#define GRAVITY -10

#include "gama/gama.hpp"
#include "gama/Shape.hpp"


Cube* ground;
Sphere* ball;


//subliminal:headers
void jump();
int render(Gama *gama);
//endsubliminal:headers

void jump()
{
    ball->pos->vel->y = 5;
}

class KeyHandler: public KeyBoardEventHandler {
public:
    void on_arrowkeydown(ArrowKey key) {
        switch(key) {
        case ArrowKey::UP:
            jump();
            break;
        default:
            break;
        }
    };
};


int load(Gama* gama)//
{
    gama->keyboardHandler = new KeyHandler();
    return 1;
}

int init(Gama* gama)//
{
    gluPerspective(2, 5, 0.5f, 10.0f);
    gama->title = "Jumpy ball";
    gama->width = 700;
    gama->height = 700;

    ground = new Cube(new Vector(0, -0.9, 0), new Vector(1, 0, 1));
    ground->color->set(DARKGREEN);
    ground->rotation->x = 350;
    gama->clearColor->set(GRAY);

    ball = new Sphere(new Vector(0, 0, 0), new Color(ORANGE), 0.1);
    ball->color->set(BLUE);
    ball->pos->acc->y = GRAVITY;
    ball->pos->y = 1;

    gama->lights.push_back(new Light(
        0,                  // Light id from 0-7
        LightType::DIFFUSE, // LightType enum: DIFFUSE|AMBIENT|SPECULAR
        255, 255, 255,      // light color
        0.01f,              // intensity
        0.0, 0.0, -1.0      // light position
    ));
    ball->rotation->vel->x = 0.1;
    ball->rotation->vel->y = 0.1;
    return 1;
}
int update(Gama* gama, double theta)//
{
    if((ball->pos->y-ball->radius+ball->radius/10) < ground->pos->y) {
        if(ball->pos->vel->y < 0)
            ball->pos->vel->y *= -0.6;
    }
    ball->update(theta);
    ground->update(theta);
    return 1;
}

int render(Gama *gama)
{
    ground->render();
    ball->render();
    return 1;
}
