#ifndef GAMA_IMAGE_INCLUDED
#define GAMA_IMAGE_INCLUDED

#include "vector.h"
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/image.h"

typedef struct {
  int width, height;
  unsigned char *data;
  unsigned int texture_id;
} Image;

Image *openImageFile(const char *path) {
  int width, height, n;
  unsigned char *data = stbi_load(path, &width, &height, &n, 4);
  if (data == NULL) {
    return NULL;
  }
  Image *img = (Image *)malloc(sizeof(Image));
  img->data = data;
  img->width = width;
  img->height = height;
  glGenTextures(1, &img->texture_id);
  return img;
}

void bindImage(Image *image) {
  glBindTexture(GL_TEXTURE_2D, image->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawImage(Image *image, Pos *pos, Pos *size) {
  glEnable(GL_TEXTURE_2D);
  bindImage(image);
  glBegin(GL_QUADS);
  {
    glTexCoord2f(0, 0);
    glVertex2f(pos->x, pos->y);
    glTexCoord2f(1, 0);
    glVertex2f(pos->x + size->x, pos->y);
    glTexCoord2f(1, 1);
    glVertex2f(pos->x + size->x, pos->y + size->y);
    glTexCoord2f(0, 1);
    glVertex2f(pos->x, pos->y + size->y);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void freeImage(Image *image) {
  stbi_image_free(image->data);
  free(image);
}

#endif // GAMA_IMAGE_INCLUDED
