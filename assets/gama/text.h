#ifndef GAMA_TEXT_INCLUDED
#define GAMA_TEXT_INCLUDED

#include "color.h"
#include <string.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/truetype.h"
#include "vector.h"

typedef struct {
  unsigned char ttf_buffer[1 << 20];
  unsigned char temp_bitmap[512 * 512];

  stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
  GLuint ftex;
} Font;

typedef struct {
  char *text;
  int len;
  double fontsize;
  Font *font;
  Vector *pos;
  Color color;
} Text;

void setText(Text *t, char *txt);

Font *newFont() {
  Font *f = (Font *)malloc(sizeof(Font));
  f->ftex = 0;
  return f;
}

Text *newText() {
  Text *t = (Text *)malloc(sizeof(Text));
  return t;
}

Font *loadFont(const char *url) {
  Font *f = newFont();
  fread(f->ttf_buffer, 1, 1 << 20, fopen(url, "rb"));

  stbtt_BakeFontBitmap(f->ttf_buffer, 0, 32.0, f->temp_bitmap, 512, 512, 32, 96,
                       f->cdata); // no guarantee this fits!
                                  // can free ttf_buffer at this point
  glGenTextures(1, &f->ftex);
  glBindTexture(GL_TEXTURE_2D, f->ftex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, f->temp_bitmap);
  // can free temp_bitmap at this point
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  return f;
}

Text *createText(char *text, Font *f, Pos *pos) {
  Text *t = newText();

  setText(t, text);
  t->fontsize = 0.1;
  t->font = f;
  t->pos = vectorAt(pos);
  return t;
}

void renderText(Text *t) {
  float x = t->pos->pos->x, y = t->pos->pos->y;
  SetGLColor(t->color);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, t->font->ftex);
  glBegin(GL_QUADS);
  for (int i = 0; i < t->len; i++) {
    if ((int)t->text[i] >= 32 && (int)t->text[i] < 128) {
      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(t->font->cdata, 512, 512, (int)t->text[i] - 32, &x, &y,
                         &q, 1); // 1=opengl & d3d10+,0=d3d9
      double scale = t->fontsize / 20;
      glTexCoord2f(q.s0, q.t0);
      glVertex2f(t->pos->pos->x + q.x0 * scale, t->pos->pos->y - q.y0 * scale);
      glTexCoord2f(q.s1, q.t0);
      glVertex2f(t->pos->pos->x + q.x1 * scale, t->pos->pos->y - q.y0 * scale);
      glTexCoord2f(q.s1, q.t1);
      glVertex2f(t->pos->pos->x + q.x1 * scale, t->pos->pos->y - q.y1 * scale);
      glTexCoord2f(q.s0, q.t1);
      glVertex2f(t->pos->pos->x + q.x0 * scale, t->pos->pos->y - q.y1 * scale);
    }
  }
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

void setText(Text *t, char *txt) {
  char *p = txt;
  int l;
  while (p) {
    ++p;
    ++l;
  }
  t->len = l;

  if (t->text != NULL)
    free(t->text);
  t->text = (char *)malloc(l * sizeof(char));
  for (int i = 0; i < l; i++)
    t->text[i] = txt[i];
}

#endif
