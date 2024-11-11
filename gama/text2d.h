#ifndef GAMA_TEXT2D_H_INCLUDED
#define GAMA_TEXT2D_H_INCLUDED

#include <string.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "vector.h"

#define F_Times() GetFont("c:/windows/fonts/times.ttf", "times")

typedef struct Font {
    char* name;
    unsigned char ttf_buffer[1<<20];
    unsigned char temp_bitmap[512*512];

    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    GLuint ftex;
} *Font;

typedef struct Text2d {
    char* text;
    int len;
    double fontsize;
    Font font;
    Vector* pos;
    Color color;
} *Text2d;

Font _g_make_font() {
    Font f = (Font)malloc(sizeof(struct Font));
    return f;
}
Text2d _g_make_text2() {
    Text2d t = (Text2d)malloc(sizeof(struct Text2d));
    return t;
}

Font GetFont(const char* url, const char* name) {
    Font f = _g_make_font();
    f->name = name;
    fread(f->ttf_buffer, 1, 1<<20, fopen(url, "rb"));

    stbtt_BakeFontBitmap(f->ttf_buffer,0, 32.0, f->temp_bitmap,512,512, 32,96, f->cdata); // no guarantee this fits!
   // can free ttf_buffer at this point
    glGenTextures(1, &f->ftex);
    glBindTexture(GL_TEXTURE_2D, f->ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, f->temp_bitmap);
   // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return f;
}

Text2d CreateText(char* text, Font f, double x, double y) {
    Text2d t = _g_make_text2();
    int e;
    for(e = 0;text[e] != '\0';e++);
    t->text = text;
    t->len = e;
    t->fontsize = 0.1;
    t->font = f;
    t->pos = Vector2(x, y);
    return t;
}

void GRenderText(Text2d t) {
    double x = t->pos->x, y = t->pos->y;
    //glColor3fv(ColorDecomposef(t->color));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t->font->ftex);
    glBegin(GL_QUADS);
    for(int i = 0;i < t->len;i++) {
        if ((int)t->text[i] >= 32 && (int)t->text[i] < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(t->font->cdata, 512,512, (int)t->text[i]-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
         double scale = t->fontsize/20;
         glTexCoord2f(q.s0,q.t0); glVertex2f(t->pos->x+q.x0*scale,t->pos->y-q.y0*scale);
         glTexCoord2f(q.s1,q.t0); glVertex2f(t->pos->x+q.x1*scale,t->pos->y-q.y0*scale);
         glTexCoord2f(q.s1,q.t1); glVertex2f(t->pos->x+q.x1*scale,t->pos->y-q.y1*scale);
         glTexCoord2f(q.s0,q.t1); glVertex2f(t->pos->x+q.x0*scale,t->pos->y-q.y1*scale);
      }
    }
   glEnd();
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
}

void GTSetText(Text2d t, const char* txt) {
    int l;
    for(l = 0;txt[l] != '\0';l++);
    t->len = l;
    free(t->text);
    t->text = (char*)malloc(l*sizeof(char));
    for(int i = 0; i < l;i++)
        t->text[i] = txt[i];
}

#endif // GAMA_TEXT2D_H_INCLUDED
