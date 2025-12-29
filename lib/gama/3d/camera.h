#include "position.h"
typedef struct {
  gm3Pos position;
  double focal;
} gm3Camera;

int gm3_camera_create(gm3Camera *c) {
  if (!c)
    return -1;
  gm3_pos_reset(&c->position);

  c->focal = 1;
  return 0;
}
