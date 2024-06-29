#ifndef GAMA_FPS_PRECESION
    #define GAMA_FPS_PRECESION 10
#endif
#include "Color.hpp"
#include "Vector.hpp"
#include "Light.hpp"
#include "Event.hpp"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>
#include <time.h>

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

        double fps_target = 60;

        void add_fps_interval(double val)
        {
            for(int i = 0; i < GAMA_FPS_PRECESION-1; i++)
            {
                fps_intervals[i] = fps_intervals[i+1];
            }
            fps_intervals[GAMA_FPS_PRECESION-1] = val;
        }
        double fps() const
        {
            double sum = 0;
            int n = 0;
            for(int i = 0; i < GAMA_FPS_PRECESION; i++)
            {
                if(fps_intervals[i] > 0)
                {
                    sum += fps_intervals[i];
                    n++;
                }
            }
            return 1.0 / (sum / n);
        }
    private:
        double fps_intervals[GAMA_FPS_PRECESION];
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
