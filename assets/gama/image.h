#ifndef GAMA_IMAGE_INCLUDED
#define GAMA_IMAGE_INCLUDED

#include "vector.h"
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/image.h"

typedef struct {
  unsigned int width, height;
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

Image *newImage(unsigned int width, unsigned int height) {
  Image *img = (Image *)malloc(sizeof(Image));
  img->data =
      (unsigned char *)malloc(width * height * 4 * sizeof(unsigned char));
  img->width = width;
  img->height = height;
  glGenTextures(1, &img->texture_id);
  return img;
}

Image *cropImage(const Image *img, unsigned int startx, unsigned int starty,
                 unsigned int width, unsigned int height) {
  Image *cropped = newImage(width, height);
  unsigned int orig, dest;
  // unsigned int endx = startx + width, endy = starty + height;

  for (unsigned int x = 0; x < width; x++) {
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int p = 0; p < 4; p++) {
        orig = ((y + starty) * img->width * 4) + (x + startx) * 4 + p;
        dest = ((y * width) + x) * 4 + p;
        if (orig > 4 * img->width * img->height) {
          cropped->data[dest] = 0;
        } else {
          cropped->data[dest] = img->data[orig];
        }
      }
    }
  }
  return cropped;
}

void bindImage(Image *image) {
  glBindTexture(GL_TEXTURE_2D, image->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawImage(Image *image, Pos *pos, Pos *size) {
  float width = size->x, height = size->y;
  float ratio = (float)image->height / (float)image->width;
  if (width == 0) {
    if (height == 0) {
      width = 0.1;
      height = 0.1 * ratio;
    } else {
      width = height / ratio;
    }
  } else if (height == 0) {
    height = width * ratio;
  }
  glColor4f(1, 1, 1, 1);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  bindImage(image);
  glBegin(GL_QUADS);
  {
    glTexCoord2f(0, 1);
    glVertex2f(pos->x, pos->y);
    glTexCoord2f(1, 1);
    glVertex2f(pos->x + width, pos->y);
    glTexCoord2f(1, 0);
    glVertex2f(pos->x + width, pos->y + height);
    glTexCoord2f(0, 0);
    glVertex2f(pos->x, pos->y + height);
  }
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

void freeImage(Image *image) {
  stbi_image_free(image->data);
  free(image);
}

#endif // GAMA_IMAGE_INCLUDED
