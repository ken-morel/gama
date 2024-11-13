#include <stdio.h>
#include "gama.h"
#include "gama/text2d.h"


Shape2d container;
Text2d label;
Font font;
void init() {
    container = CreateRoundRectangle(0, 0, 1, 0.5, 0.1);
    container->color = DARKGREEN;
    font = F_Times();
    label = CreateText("Enter\0", font, 0, 0);
    label->fontsize = 0.1;
    label->color = BLUE;
}

void update(float theta) {
    char text[] = "\0\0\0\0\0\0";
    if((int)theta % 10 == 0) {
        sprintf(text, "%.2f", gfps());
        GTSetText(label, text);
    }
    //printf(text);
}

void render() {
    G2Render(container);
    GRenderText(label);
}
