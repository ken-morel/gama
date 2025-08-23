#ifndef GAMA_SPRITE_INCLUDED
#define GAMA_SPRITE_INCLUDED

#include "vector.h"
#define STB_IMAGE_IMPLEMENTATION
#include "image.h"

typedef struct {
  Vector *pos;
  Pos *size;
  int current;
  int length;
  Image *images;
} Sprite;

#endif // GAMA_SPRITE_INCLUDED
