#include <assets/gltf/cube.glb.h>
#include <gama.h>
#include <gama/3d.h>
#include <gama/3d/gltf.h>
#include <gama/debug.h>

int main() {

  gm_init(0, 0, "Hello world");
  gm_show_fps(1);

  gm_fullscreen(1);

  gm3Mesh mesh = cube_mesh();

  gm3Transform trans = gm3_transform();
  trans.position.z = 5;

  gm_background(0x333355FF);

  gm3Image img = gm3_image();

  do {
    if (gm_mouse.down) {
      trans.rotation.x += gm_mouse.movement.y;
      trans.rotation.y -= gm_mouse.movement.x;
    }
    gm3_project(&img, &mesh, &trans, NULL);

    gm3_draw_image(&img, 0, 0, 1);

    gm3_image_reset(&img);
  } while (gm_yield());
  return 0;
}
