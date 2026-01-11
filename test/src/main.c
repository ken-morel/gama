#include "gama/image.h"
#include "gama/sprite.h"
#include <assets/images/icon.png.h>
#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(0, 0, "Hello world");

  gm_show_fps(1);

  gmImage icon = icon_image();

  gmSprite sprite = gm_sprite_create(icon, 5);

  gm_background(0x333355FF);

  gm_show_fps(1);

  do {
    gm_sprite_update(&sprite);
    gm_image_draw(icon, 0, 0, 1, 1);
  } while (gm_yield());
  return 0;
}
