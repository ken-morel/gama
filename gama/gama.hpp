#include "Color.hpp"
#include "Vector.hpp"
#include "Light.hpp"
#include "Event.hpp"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>


//subliminal:headers

//endsubliminal:headers


class Gama
{
    public:
        Gama();
        void resize(int w, int h);

        Color *clearColor, *lightColor;
        int width = 500, height = 500;
        double depth;
        std::vector<Light*> lights;
        std::string title = "gama sample";
        KeyBoardEventHandler* keyboardHandler;
};

Gama::Gama():clearColor(0), lightColor(0)
{
    clearColor = new Color(255, 255, 255);
    lightColor = new Color(255, 255, 255);
    depth = 100.0f;
    resize(width, width);
}

void Gama::resize(int w, int h)
{
    width = w;
    height = h;
}

#include "main.hpp"
