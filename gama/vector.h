#ifndef GAMA_VECTOR_H_INCLUDED
#define GAMA_VECTOR_H_INCLUDED


typedef struct {
    double x, y, z;
} Pos;

typedef struct {
    Pos* v;
    Pos* a;
    double x, y, z;
} Vector;

Pos* _make_pos() {
    Pos* p = (Pos*)malloc(sizeof(Pos));
    p->x = 0;
    p->y = 0;
    p->z = 0;
    return p;
}

Vector* _make_vector() {
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->v = _make_pos();
    v->a = _make_pos();
    v->x = 0;
    v->y = 0;
    v->z = 0;
    return v;
}

Vector* Vector2(double x, double y) {
    Vector* v = _make_vector();
    v->x = x;
    v->y = y;
    return v;
}
Vector* Vector3(double x, double y, double z) {
    Vector* v = _make_vector();
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}
void VUpdate(Vector* v, float t) {
    if(v == NULL) return;
    v->x += v->v->x*t;
    v->y += v->v->y*t;
    v->z += v->v->z*t;

    v->v->x += v->a->x*t;
    v->v->y += v->a->y*t;
    v->v->z += v->a->z*t;
}

#endif // GAMA_VECTOR_H_INCLUDED
