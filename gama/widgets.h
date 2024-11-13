#ifndef GAMA_WIDGETS_H_INCLUDED
#define GAMA_WIDGETS_H_INCLUDED

typedef struct GWidget {
    unsigned int type;
    unsigned long int id;
} *GWidget;

typedef struct GWClass {
    unsigned int id;
    char name[15];
    int (*init)(GWidget);
    int (*update)(GWidget);
    int (*render)(GWidget);
} *GWClass;

GWClass *GWClasses = 0;
int NGWClasses = 0;


int GWCAdd(int (*init)(GWidget), int (*update)(GWidget), int (*render)(GWidget)) {
    GWClass wclass = (GWClass)malloc(sizeof(struct GWClass));
    wclass->id = NGWClasses;
    wclass->init = init;
    wclass->update = update;
    wclass->render = render;
    GWClass *nclasses = (GWClass)malloc((NGWClasses + 1) * sizeof(struct GWClass));
    for(int i = 0; i < NGWClasses;i++)
        nclasses[i] = GWClasses[i];
    free(GWClasses);
    nclasses[NGWClasses+1] = wclass;
    GWClasses = nclasses;
    return NGWClasses++;
}

int WIDGET_ID = 0;

int GWInit(GWidget w) {
    GWClasses[w->type].init(w);
}
int GWInit(GWidget w) {
    GWClasses[w->type].init(w);
}

GWidget CreateWidget(int id) {
    GWidget w = (GWidget)malloc(sizeof(struct GWidget));
    w->id = WIDGET_ID++;
    w->type = id;

}


#endif // WIDGETS_H_INCLUDED
