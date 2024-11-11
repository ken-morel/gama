#ifndef GAMA_H_INCLUDED
#define GAMA_H_INCLUDED

#include <gl/gl.h>
#include <time.h>

#ifndef WINDOW_WIDTH
    #define WINDOW_WIDTH 500
#endif
#ifndef WINDOW_HEIGHT
    #define WINDOW_HEIGHT WINDOW_WIDTH
#endif
#ifndef WINDOW_TITLE
    #define WINDOW_TITLE "Gama Sample"
#endif
#ifndef SPEED
    #define SPEED 0.01
#endif

#include "gama/color.h"
#include "gama/shape2d.h"

Color CLEARCOLOR = 0x0;
int ltime;

void init(void);
void update(float);
void render(void);

double fps_intervals[20];

void gama_init() {
    ltime = clock();
    fps_intervals[0] = ltime;
    for(int i = 1; i < 20;i++)
        fps_intervals[i] = 0;
    init();
}
void gama_update() {
    float dt = (float)(clock()-ltime)/1000.0f;
    for(int i = 0; i < 19;i++)
        fps_intervals[i] = fps_intervals[i+1];
    fps_intervals[0] = dt;
    update(dt * SPEED);
}
void gama_render() {
    render();
}

double gfps() {
    float sum = 0;
    int n = 0;
    for(n = 0; n < 20;n++) {
        if(fps_intervals[n] == 0)
            break;
        sum += fps_intervals[n];
    }
    return sum / n;
}




#include "gama/win32.h"

#endif // GAMA_H_INCLUDED
