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
  unsigned int length;
  Image **images;

  float fps;

  unsigned int animationCurrent;
  unsigned int animationLen;
  unsigned int *animation;
  unsigned int width;
  unsigned int height;
  float _remainingTimeFrame;
} Sprite;

Sprite *openSprite(const char *path, unsigned int width, unsigned int height,
                   Pos *pos, Pos *size) {
  Image *img = openImageFile(path);
  if (img == NULL) {
    printf("Failed to load image at %s to create sprite", path);
    return NULL;
  }
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
  sprite->pos = vectorAt(pos);
  sprite->animationCurrent = 0;
  sprite->length = img->width / img->height;
  sprite->animationLen = sprite->length;
  sprite->animation =
      (unsigned int *)malloc(sprite->length * sizeof(unsigned int));
  for (size_t i = 0; i < sprite->length; i++)
    sprite->animation[i] = i;
  sprite->size = size;
  sprite->images = (Image **)malloc(sprite->length * sizeof(Image *));
  sprite->fps = 4.0f;
  sprite->_remainingTimeFrame = 1.0f / sprite->fps;
  sprite->width = width;
  sprite->height = height;
  for (size_t idx = 0; idx < sprite->length; idx++) {
    sprite->images[idx] = cropImage(img, idx * width, 0, width, height);
  }
  return sprite;
}
void updateSprite(Sprite *s, float theta) {
  theta += s->_remainingTimeFrame; // time since last frame time
  float timeForOneFrame = (1.0f / s->fps);
  float skipping =
      theta / timeForOneFrame; // number of frames which actually passed
  float rounded = floor(skipping);
  s->animationCurrent += (int)rounded;
  s->animationCurrent %= s->animationLen;

  s->_remainingTimeFrame = theta - (rounded * timeForOneFrame);
  updateVector(s->pos, theta);
}

void renderSprite(Sprite *s) {
  s->animationCurrent %= s->animationLen;
  int currentFrame = s->animation[s->animationCurrent];
  currentFrame %= s->length;
  Image *frame = s->images[currentFrame];
  drawImage(
      frame,
      at(s->pos->pos->x - s->size->x / 2, s->pos->pos->y - s->size->y / 2),
      s->size);
}

void setSpriteAnimation(Sprite *s, unsigned int length, unsigned int *animation,
                        unsigned int fps) {
  if (s->animation != NULL)
    free(s->animation);
  s->animation = (unsigned int *)malloc(length * sizeof(unsigned int));
  s->animationLen = length;
  s->animationCurrent = 0;
  s->fps = fps;
  for (size_t i = 0; i < length; i++) {
    s->animation[i] = animation[i];
  }
}
#define SetSpriteAnimationArray(s, array, fps)                                 \
  setSpriteAnimation(s, sizeof(array) / sizeof(unsigned int), array, fps)

//////////////////////
void setSpriteVelocity(Sprite *s, Pos *vel) {
  free(s->pos->vel);
  s->pos->vel = vel;
}
Pos *getSpriteVelocity(Sprite *s) { return s->pos->vel; }
Pos *getSpritePosition(Sprite *s) { return s->pos->pos; }
Pos *getSpriteAcceleration(Sprite *s) { return s->pos->acc; }

void setSpriteAcceleration(Sprite *s, Pos *acc) {
  free(s->pos->acc);
  s->pos->acc = acc;
}

void setSpritePosition(Sprite *s, Pos *pos) {
  free(s->pos->pos);
  s->pos->pos = pos;
}

typedef struct {
  Sprite **sprites;
  int length;
} SpriteList;

SpriteList *newSpriteList() {
  SpriteList *list = (SpriteList *)malloc(sizeof(SpriteList));
  list->length = 0;
  list->sprites = NULL;
  return list;
}

void addSpriteToList(SpriteList *list, Sprite *sprite) {
  int i;
  Sprite **newSprites =
      (Sprite **)malloc((list->length + 1) * sizeof(Sprite *));
  for (i = 0; i < list->length; i++)
    newSprites[i] = list->sprites[i];
  newSprites[list->length] = sprite;
  free(list->sprites);
  list->sprites = newSprites;
}

void renderSprites(SpriteList *list) {
  int i;
  for (i = 0; i < list->length; i++) {
    renderSprite(list->sprites[i]);
  }
}
void updateSprites(SpriteList *list, float theta) {
  int i;
  for (i = 0; i < list->length; i++) {
    updateSprite(list->sprites[i], theta);
  }
}

float spriteTop(Sprite *s) { return s->pos->pos->y + s->size->y / 2.0f; }
float spriteBottom(Sprite *s) { return s->pos->pos->y - s->size->y / 2.0f; }
float spriteLeft(Sprite *s) { return s->pos->pos->x - s->size->x / 2.0f; }
float spriteRight(Sprite *s) { return s->pos->pos->x + s->size->x / 2.0f; }

float bounceSpriteUnder(Sprite *s, float pos, float bounce, float maxSpeed) {
  maxSpeed = -abs(maxSpeed);
  float halfHeight = s->size->y / 2;
  if (s->pos->pos->y - halfHeight <= pos) {
    s->pos->pos->y = pos + halfHeight;
    if (s->pos->vel->y < 0) {
      s->pos->vel->y = s->pos->vel->y < maxSpeed ? maxSpeed : s->pos->vel->y;
      s->pos->vel->y *= -bounce;
    }
  }
}

void bounceSprite(Sprite *s, float x, float y) {
  if (x != 0) {
    s->pos->vel->x *= -x;
  }
  if (y != 0) {
    s->pos->vel->y *= -y;
  }
}

#endif // GAMA_SPRITE_INCLUDED
