//subliminal:headers
void jump();
//endsubliminal:headers

#define WIDTH 700
#define HEIGHT WIDTH
#include "gama/gama.hpp"
#include "gama/Shape.hpp"

Cube *cube, *ground;

void jump()
{
    cube->pos->vel->y += 0.5;
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
    cube = new Cube(new Vector(0, 0, 0), new Vector(0.5, 0.5, 0.5));
    ground = new Cube(new Vector(0, -0.9, 1), new Vector(1, 0, 1));
    ground->color->set(DARKGREEN);
    ground->rotation->x = 10;
    cube->color->set(GREEN);
    cube->rotation->vel->x = 1.0f;
    cube->rotation->vel->y = 0.1f;
    cube->rotation->vel->z = 0.1f;
    gama->clearColor->set(GRAY);

    //gama->lights.push_back(new Light(0, LightType::DIFFUSE, 0, 255, 255, 0, 0, -1));
    //gama->lights.push_back(new Light(1, LightType::SPECULAR, 255, 255, 255, 0, 0, -1));
    return 1;
}
int update(Gama* gama)//
{
    cube->update(1);
    ground->update(1);
    return 1;
}
int theta;
int render(Gama* gama)//
{
    //glRotatef(theta, theta, theta, 1.0f);
    theta++;


    //cube -> rotation -> disable();
    cube -> render();

    ground -> render();

    // End of drawing color-cube


    return 1;
}
