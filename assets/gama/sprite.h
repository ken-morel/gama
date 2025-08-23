#ifndef GAMA_SPRITE_INCLUDED
#define GAMA_SPRITE_INCLUDED

#include "math.h"
#include "vector.h"
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "image.h"

typedef struct {
  Vector *pos;
  Pos *size;
  size_t length;
  Image **images;

  float fps;

  unsigned int animationCurrent;
  unsigned int animationLen;
  unsigned int *animation;
  float _remainingTimeFrame;
} Sprite;

Sprite *openSprite(const char *path, unsigned int width, unsigned int height) {
  Image *img = openImageFile(path);
  if (img == NULL)
    return NULL;
  if (img->width % width != 0) {
    printf("Invalid sprite file width %d not multiple specified width %d."
           " Thus the image cannot be equaly cut, when loading %s",
           img->width, width, path);
    freeImage(img);
    return NULL;
  }
  if (img->height != height) {
    printf("Image height %d does not match specified height %d. Cannot slice "
           "sprite %s",
           img->height, height, path);
    freeImage(img);
    return NULL;
  }
  Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
  sprite->pos = vectorAt(at(0, 0));
  sprite->animationCurrent = 0;
  sprite->length = img->width / img->height;
  sprite->size = at(width, height);
  sprite->images = (Image **)malloc(sprite->length * sizeof(Image *));
  sprite->fps = 4;
  sprite->_remainingTimeFrame = sprite->fps;
  for (size_t idx = 0; idx < sprite->length; idx++) {
    sprite->images[idx] = cropImage(img, idx * sprite->size->x, 0,
                                    sprite->size->x, sprite->size->y);
  }
  return sprite;
}
void updateSprite(Sprite *s, float theta) {
  theta += s->_remainingTimeFrame; // time since last frame time
  float skipping = theta / s->fps;
  float rounded = (int)floor(skipping);
  s->animationCurrent = (s->animationCurrent + (int)rounded) % s->animationLen;
  s->_remainingTimeFrame = theta - (rounded * s->fps);
  updateVector(s->pos, theta);
}

#endif // GAMA_SPRITE_INCLUDED
