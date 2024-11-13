

#ifndef GAMA_WIDGETS_FPS_VIEW_H_INCLUDED
#define GAMA_WIDGETS_FPS_VIEW_H_INCLUDED

#include "../shape2d.h"
#include "../widgets.h"
#include "../text2d.h"

Shape2d container;
Text2d label;
Font font;
int FPS_VIEW = -1;

void tinit() {
    container = CreateRoundRectangle(0, 0, 1, 0.5, 0.1);
    container->color = DARKGREEN;
    font = F_Times();
    label = CreateText("Enter\0", font, 0, 0);
    label->fontsize = 0.1;
    label->color = BLUE;
}
void tupdate(float theta) {
    char text[] = "\0\0\0\0\0\0";
    if((int)theta % 10 == 0) {
        sprintf(text, "%.2f", gfps());
        GTSetText(label, text);
    }
}
void trender() {
    G2Render(container);
    GRenderText(label);
}

void __fps_view_init() {
    FPS_VIEW = GWCAdd(tinit, tupdate, trender);
}

#endif // FPS_VIEW_H_INCLUDED
