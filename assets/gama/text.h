#ifndef GAMA_TEXT_INCLUDED
#define GAMA_TEXT_INCLUDED

#include "color.h"
#include <stdio.h>
#include <string.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/truetype.h"
#include "vector.h"

#define TEXT_DPI 512

typedef struct {
  unsigned char ttf_buffer[1 << 20];
  unsigned char temp_bitmap[TEXT_DPI * TEXT_DPI];

  stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
  GLuint ftex;
} Font;

typedef struct {
  char *text;
  int len;
  double fontsize;
  Font *font;
  Vector pos;
  Color color;
} Text;

int setTextN(Text *, const char *, size_t);
int setText(Text *, const char *);

void updateText(Text *t, double theta) { updateVector(&t->pos, theta); }
Font *_newFont() {
  Font *f = (Font *)malloc(sizeof(Font));
  f->ftex = 0;
  return f;
}

Text *newText() {
  Text *t = (Text *)malloc(sizeof(Text));
  t->text = NULL;
  t->color = WHITE;
  t->font = NULL;
  t->len = 0;
  t->fontsize = 0;
  return t;
}

Font *loadFont(const char *url) {
  Font *f = _newFont();
  // It's safer to check if fopen was successful
  FILE *fontFile = fopen(url, "rb");
  if (!fontFile) {
    printf("Error: Could not open font file at %s\n", url);
    free(f);
    return NULL;
  }
  fread(f->ttf_buffer, 1, 1 << 20, fontFile);
  fclose(fontFile);

  stbtt_BakeFontBitmap(f->ttf_buffer, 0, 32.0, f->temp_bitmap, TEXT_DPI,
                       TEXT_DPI, 32, 96,
                       f->cdata); // no guarantee this fits!
                                  // can free ttf_buffer at this point
  glGenTextures(1, &f->ftex);
  glBindTexture(GL_TEXTURE_2D, f->ftex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TEXT_DPI, TEXT_DPI, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, f->temp_bitmap);
  // can free temp_bitmap at this point
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  return f;
}
Font *loadFontAsset(const char *name) {
  char path[100];
  sprintf(path, "assets/fonts/%s.ttf", name);
  return loadFont(path);
}

Text *createTextN(const char *text, size_t length, double fontsize, Font *f,
                  Pos pos) {
  Text *t = newText();

  setTextN(t, text, length);
  t->fontsize = fontsize;
  t->font = f;
  t->pos = VectorAt(pos);
  return t;
}
Text *createText(const char *text, double fontsize, Font *f, Pos pos) {
  Text *t = newText();
  setText(t, text);
  t->fontsize = fontsize;
  t->font = f;
  t->pos = VectorAt(pos);
  return t;
}

float getTextWidth(Text *t) {
  if (!t || !t->text || !t->font) {
    return 0.0f;
  }

  float total_width = 0.0f;
  float x = 0.0f, y = 0.0f; // Dummy cursor position for width calculation

  for (int i = 0; i < t->len; i++) {
    if ((int)t->text[i] >= 32 && (int)t->text[i] < 128) {
      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(t->font->cdata, TEXT_DPI, TEXT_DPI,
                         (int)t->text[i] - 32, &x, &y, &q, 1);
    }
  }

  double scale = t->fontsize / 20.0;
  total_width =
      x * scale; // The final x position is the width in pixels, so we scale it.

  return total_width;
}

int renderText(Text *t) {
  if (!t)
    return 0;
  else if (!t->text)
    return -1;
  else if (!t->font)
    return -2;
  float x = 0, y = 0;

  float totalWidth = getTextWidth(t);

  float offset = totalWidth / 2.0f;

  double scale = t->fontsize / 20.0;

  SetGLColor(t->color);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, t->font->ftex);

  glTranslatef(t->pos.x - offset, t->pos.y, 0);
  glBegin(GL_QUADS);

  for (int i = 0; i < t->len; i++) {
    if ((int)t->text[i] >= 32 && (int)t->text[i] < 128) {
      stbtt_aligned_quad q;
      // stbtt_GetBakedQuad updates 'x' and 'y' to position the next character
      stbtt_GetBakedQuad(t->font->cdata, 512, 512, (int)t->text[i] - 32, &x, &y,
                         &q, 1);

      // 2. BUG FIX: Use the 'q' coordinates directly for the vertices.
      // Do not add the original t->pos->pos->x to them.
      // We apply your custom scale and y-inversion.
      glTexCoord2f(q.s0, q.t0);
      glVertex2f(q.x0 * scale, q.y0 * -scale);
      glTexCoord2f(q.s1, q.t0);
      glVertex2f(q.x1 * scale, q.y0 * -scale);
      glTexCoord2f(q.s1, q.t1);
      glVertex2f(q.x1 * scale, q.y1 * -scale);
      glTexCoord2f(q.s0, q.t1);
      glVertex2f(q.x0 * scale, q.y1 * -scale);
    }
  }
  glEnd();
  glTranslatef(offset - t->pos.x, -t->pos.y, 0);

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  return 1;
}

int setTextN(Text *t, const char *txt, size_t length) {
  if (!t)
    return 0;
  if (t->text) {
    free(t->text);
    t->text = NULL;
  }

  t->text = (char *)malloc(length + 1);
  if (t->text == NULL) {
    t->len = 0;
    printf("Could not allocate text space");
    return -10;
  }

  memcpy(t->text, txt, length);
  t->text[length] = '\0';
  t->len = length;
  return 1;
}

int setText(Text *t, const char *txt) {
  if (!t)
    return 0;
  if (t->text != NULL) {
    free(t->text);
    t->text = NULL;
  }
  int len;
  for (len = 0; txt[len] != '\0'; len++)
    if (len > 0 && len % 1000 == 0)
      printf("String possibly non-terminated(passed %d characters)", len);

  t->text = (char *)malloc(len + 1);
  if (t->text == NULL) {
    t->len = 0;
    printf("Could not allocate text space");
    return -10;
  }

  memcpy(t->text, txt, len);
  t->text[len] = '\0';
  t->len = len;
  return 1;
}

#endif
