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

void gama_init() {
    ltime = clock();
    init();
}
void gama_update() {
    float dt = (float)(clock()-ltime)/1000.0f;
    update(dt * SPEED);
}
void gama_render() {
    render();
}




#include "gama/win32.h"

#endif // GAMA_H_INCLUDED
