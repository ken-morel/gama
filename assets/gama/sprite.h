#ifndef GAMA_SPRITE_INCLUDED
#define GAMA_SPRITE_INCLUDED

#include "math.h"
#include "vector.h"
#include <stdio.h>
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
  double _remainingTimeFrame;
} Sprite;

Sprite *newSprite() {
  Sprite *s = (Sprite *)malloc(sizeof(Sprite));
  return s;
}

Sprite *createSprite(const char *path, unsigned int width, unsigned int height,
                     Pos *pos, Pos *size) {
  Sprite *sprite = newSprite();
  if (sprite == NULL) {
    printf("Could not create sprite object");
    return NULL;
  }
  Image *img = newImage();
  openImageFile(img, path);
  if (img == NULL) {
    printf("Failed to load image at %s to create sprite", path);
    fflush(stdout);
    return NULL;
  } else if (img->width % width != 0) {
    printf("Invalid sprite file width %d not multiple specified width %d."
           " Thus the image cannot be equaly cut, when loading %s",
           img->width, width, path);
    fflush(stdout);
    freeImage(img);
    return NULL;
  } else if (img->height != height) {
    printf("Image height %d does not match specified height %d. Cannot slice "
           "sprite %s",
           img->height, height, path);
    fflush(stdout);
    freeImage(img);
    return NULL;
  }

  sprite->pos = vectorAt(pos);
  sprite->animationCurrent = 0;
  sprite->length = img->width / width;
  sprite->animationLen = sprite->length;
  sprite->animation =
      (unsigned int *)malloc(sprite->length * sizeof(unsigned int));
  for (size_t i = 0; i < sprite->length; i++)
    sprite->animation[i] = i;
  sprite->size = size;
  sprite->images = (Image **)malloc(sprite->length * sizeof(Image *));
  sprite->fps = 4.0f;
  sprite->_remainingTimeFrame = 1.0 / sprite->fps;
  sprite->width = width;
  sprite->height = height;
  // printf("image size %dx%d cropping %dx%d. Length is %d\n", img->width,
  //        img->height, width, height, sprite->length);
  //
  for (size_t idx = 0; idx < sprite->length; idx++) {
    sprite->images[idx] = cropImage(img, idx * width, 0, width, height);
  }
  return sprite;
}
void updateSprite(Sprite *s, double theta) {
  updateVector(s->pos, theta);
  double period = (1.0 / s->fps);
  double timeSinceLastFrame = theta + s->_remainingTimeFrame;

  double numberOfFramesSkipped = timeSinceLastFrame / period;

  int fullNumberOfFrames = (int)floor(numberOfFramesSkipped);
  double remainingFrames = numberOfFramesSkipped - (double)fullNumberOfFrames;
  double remainingTime = remainingFrames * period;

  // printf(
  //     "period is %lf. passed %lf seconds since last frame, so %lf frames(%d)
  //     " "were skipped and such %lf frames(i.e %lf seconds) are left\n",
  //     period, timeSinceLastFrame, numberOfFramesSkipped, fullNumberOfFrames,
  //     remainingFrames, remainingTime);

  s->animationCurrent += (int)fullNumberOfFrames;
  s->animationCurrent %= s->animationLen;

  s->_remainingTimeFrame = remainingTime;
}

void renderSprite(Sprite *s) {
  s->animationCurrent %= s->animationLen;
  int currentFrame = s->animation[s->animationCurrent];
  currentFrame %= s->length;
  Image *frame = s->images[currentFrame];

  drawImage(frame, s->pos->pos->x - s->size->x / 2,
            s->pos->pos->y - s->size->y / 2, s->size->x, s->size->y);
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
  if (s->pos->vel != NULL)
    free(s->pos->vel);
  s->pos->vel = vel;
}
Pos *getSpriteVelocity(Sprite *s) { return s->pos->vel; }
Pos *getSpritePosition(Sprite *s) { return s->pos->pos; }
Pos *getSpriteAcceleration(Sprite *s) { return s->pos->acc; }

void setSpriteAcceleration(Sprite *s, Pos *acc) {
  if (s->pos->acc != NULL)
    free(s->pos->acc);
  s->pos->acc = acc;
}

void setSpritePosition(Sprite *s, Pos *pos) {
  if (s->pos->pos != NULL)
    free(s->pos->pos);
  s->pos->pos = pos;
}

double spriteTop(Sprite *s) { return s->pos->pos->y + s->size->y / 2.0; }
double spriteBottom(Sprite *s) { return s->pos->pos->y - s->size->y / 2.0; }
double spriteLeft(Sprite *s) { return s->pos->pos->x - s->size->x / 2.0; }
double spriteRight(Sprite *s) { return s->pos->pos->x + s->size->x / 2.0; }

float bounceSpriteUnder(Sprite *s, double pos, double bounce, double maxSpeed) {
  maxSpeed = -abs(maxSpeed);
  double halfHeight = s->size->y / 2;
  if (s->pos->pos->y - halfHeight <= pos) {
    s->pos->pos->y = pos + halfHeight;
    if (s->pos->vel->y < 0) {
      s->pos->vel->y = s->pos->vel->y < maxSpeed ? maxSpeed : s->pos->vel->y;
      s->pos->vel->y *= -bounce;
    }
  }
}

void bounceSprite(Sprite *s, double x, double y) {
  if (x != 0) {
    s->pos->vel->x *= -x;
  }
  if (y != 0) {
    s->pos->vel->y *= -y;
  }
}

int rectsCollide(Pos *pa, Pos *sa, Pos *pb, Pos *sb) {
  double xMinDistance = (sa->x + sb->x) / 2.0;
  double yMinDistance = (sa->y + sb->y) / 2.0;

  double xDistance = pa->x - pb->x;
  double yDistance = pa->y - pb->y;

  if (xDistance < 0)
    xDistance *= -1;
  if (yDistance < 0)
    yDistance *= -1;

  return xDistance < xMinDistance && yDistance < yMinDistance;
}
int spriteTouchesSprite(Sprite *a, Sprite *b) {
  return rectsCollide(a->pos->pos, a->size, b->pos->pos, b->size);
}
#endif // GAMA_SPRITE_INCLUDED
