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

#ifndef FPS_INTERVALS
    #define FPS_INTERVALS 500
#endif // FPS_INTERVALS
Color CLEARCOLOR = 0x0;
int ltime;

void init(void);
void update(float);
void render(void);

float fps_intervals[FPS_INTERVALS];

void gama_init() {
    ltime = clock();
    for(int i = 0; i < FPS_INTERVALS;i++)
        fps_intervals[i] = 0.0f;
    init();
}
void gama_update() {
    int ctime = clock();
    float dt = (float)(ctime-ltime)/1000.0f;
    for(int i = FPS_INTERVALS - 1; i > 0;i--)
        fps_intervals[i] = fps_intervals[i-1];
    fps_intervals[0] = dt;
    ltime = ctime;
    update(dt * SPEED);
}
void gama_render() {
    render();
}

float gfps() {
    float sum = 0;
    int n = 0;
    for(n = 0; n < FPS_INTERVALS;n++) {
        if(fps_intervals[n] == 0.0f)
            break;
        sum += fps_intervals[n];
    }
    return 1.0/(sum/(float)n);
}




#include "gama/win32.h"

#endif // GAMA_H_INCLUDED
