#define GAMA_FPS_PRECESION 5000

#include "gama/gama.hpp"
#include "gama/Shape.hpp"

gRoundRectangle *r;
FPSWidget *fps;
ImageFont *font;
gCircle *circ;
int load(Gama* gama)
{
    font = new ImageFont("fonts/aclonica");
    return 1;
}
int init(Gama* gama)
{
    r = new gRoundRectangle(0, 0, 1, 1, 0.7);
    r->color->set(BLUE);
    fps = new FPSWidget(font);
    circ = new gCircle(0, 0, 0.7);
    circ->color->set(ORCHID);

    gama->lights.push_back(new Light(
        0,                  // Light id from 0-7
        LightType::DIFFUSE, // LightType enum: DIFFUSE|AMBIENT|SPECULAR
        255, 255, 255,      // light color
        0.01f,              // intensity
        0.0, 0.0, -1.0      // light position
    ));
    return 1;
}
int update(Gama* gama, double theta)
{
    fps->update(gama->fps());
    printf("%lf\n", gama->fps());
    return 1;
}
int render(Gama* gama)
{
    fps->render();
    r -> render();
    circ -> render();
    return 1;
}
