#ifndef GAMA_SHAPE2D_H_INCLUDED
#define GAMA_SHAPE2D_H_INCLUDED

#include "vector.h"
#include "color.h"
#define PI 22.0/7.0

#include <math.h>
//#include <stdarg.h>

enum ShapeType {RECT2D, CIRCLE2D, ROUNDRECT2D};

struct Shape2d {
    enum ShapeType type;
    double r;
    Vector* pos;
    Vector* s;
    Color color;
};
typedef struct Shape2d* Shape2d;

Shape2d _make_shape() {
    Shape2d s = (Shape2d)malloc(sizeof(struct Shape2d));
    s->r = 0;
    return s;
}

Shape2d CreateRectangle(double x, double y, double w, double h) {
    Shape2d s = _make_shape();
    s->r = 0;
    s->type = RECT2D;
    s->pos = Vector2(x, y);
    s->s = Vector2(w, h);
    return s;
}
Shape2d CreateRoundRectangle(double x, double y, double w, double h, double r) {
    Shape2d s = _make_shape();
    s->r = r;
    s->type = ROUNDRECT2D;
    s->pos = Vector2(x, y);
    s->s = Vector2(w, h);
    return s;
}
Shape2d CreateCircle(double x, double y, double r) {
    Shape2d s = _make_shape();
    s->r = r;
    s->type = CIRCLE2D;
    s->pos = Vector2(x, y);
    s->s = Vector2(0, 0);
    return s;
}

void G2Update(Shape2d s, float theta) {
    if(s == NULL) return;
    VUpdate(s->pos, theta);
    VUpdate(s->s, theta);
}
void drawRectangle(double x, double y, double w, double h) {
    glVertex2d(x - w/2, y + h/2);
    glVertex2d(x + w/2, y + h/2);
    glVertex2d(x + w/2, y - h/2);
    glVertex2d(x - w/2, y - h/2);
}
void drawArc(double x, double y, double r, double b, double e) {
    glVertex2d(x, y);
    for(double n = b; n <= e+1.0/7.1;n+=1.0/7.0) {
        glVertex2d(x+r*sin(n), y+r*cos(n));
    }
}
void renderRoundRectangle(Shape2d rr) {
    glColor3fv(ColorDecomposef(rr->color));
    glBegin(GL_QUADS);
        drawRectangle(rr->pos->x, rr->pos->y, rr->s->x-2*rr->r, rr->s->y);  //vertical
        drawRectangle(rr->pos->x, rr->pos->y, rr->s->x, rr->s->y-2*rr->r);  //horizontal
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        drawArc(rr->pos->x - rr->s->x/2 + rr->r, rr->pos->y + rr->s->y / 2 - rr->r, rr->r, 3*PI/2, 2*PI); //topleft
        drawArc(rr->pos->x + rr->s->x/2 - rr->r, rr->pos->y + rr->s->y / 2 - rr->r, rr->r, 0, PI/2); //topright

        drawArc(rr->pos->x - rr->s->x/2 + rr->r, rr->pos->y - rr->s->y / 2 + rr->r, rr->r, PI, 3*PI/2); //bottomleft
        drawArc(rr->pos->x + rr->s->x/2 - rr->r, rr->pos->y - rr->s->y / 2 + rr->r, rr->r, PI/2, PI); //bottomright
    glEnd();
}
void drawCircle(double x, double y, double r) {
    glVertex2d(x, y);
    for(double n = 0.0; n <= 45.0/7.0;n+=1.0/7.0) {
        glVertex2d(x+r*sin(n), y+r*cos(n));
    }
}
void G2Render(Shape2d s) {
    float* c;
    switch(s->type) {
    case RECT2D:
        glBegin(GL_QUADS);
            c = ColorDecomposef(s->color);
            glColor3fv(c);
            drawRectangle(s->pos->x, s->pos->y, s->s->x, s->s->y);
        glEnd();
        break;
    case CIRCLE2D:
        glBegin(GL_TRIANGLE_FAN);
            c = ColorDecomposef(s->color);
            glColor3fv(c);
            drawCircle(s->pos->x, s->pos->y, s->r);
        glEnd();
        break;
    case ROUNDRECT2D:
        renderRoundRectangle(s);
        break;
    }
}
double G2ShapeBottom(Shape2d s) {
    if(s == NULL) return 0;
    switch(s->type) {
    case RECT2D:
        return s->pos->y - s->s->y/2;
    case CIRCLE2D:
        return s->pos->y - s->r;
    }
    return 0;
}
double G2ShapeTop(Shape2d s) {
    if(s == NULL) return 0;
    switch(s->type) {
    case RECT2D:
        return s->pos->y + s->s->y/2;
    case CIRCLE2D:
        return s->pos->y + s->r;
    }
    return 0;
}
double G2ShapeLeft(Shape2d s) {
    if(s == NULL) return 0;
    switch(s->type) {
    case RECT2D:
        return s->pos->x - s->s->x/2;
    case CIRCLE2D:
        return s->pos->x - s->r;
    }
    return 0;
}
double G2ShapeRight(Shape2d s) {
    if(s == NULL) return 0;
    switch(s->type) {
    case RECT2D:
        return s->pos->x + s->s->x/2;
    case CIRCLE2D:
        return s->pos->x + s->r;
    }
    return 0;
}

void G2BounceX(Shape2d r, double f) {
    r->pos->v->x = -f * r->pos->v->x;
}
void G2BounceY(Shape2d r, double f) {
    r->pos->v->y = -f * r->pos->v->y;
}

int G2ShapeFalling(Shape2d r) {
    return r->pos->v->y < 0;
}

#endif // GAMA_SHAPE2D_H_INCLUDED
