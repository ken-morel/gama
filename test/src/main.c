#include <assets/obj/alien4k.obj.h>
#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(0, 0, "Hello world");

  gm_background(0x333355FF);

  gm_show_fps(1);

  gm3Mesh tree = alien4k_mesh();

  gm3Image img = gm3_image();

  gm3Transform tr = gm3_transform();
  tr.position.z = 30;
  tr.rotation.y = -M_PI * 2;

  gm3Scene scene = gm3_scene();

  fprintf(stderr, "Hello wfrom here");
  fprintf(stdout, "Hello from gama world");
  printf("Hello from gama world");

  fflush(stdout);
  fflush(stderr);

  char buff[1024];
  fscanf(stdin, "%1023s", buff);
  printf("%s", buff);

  do {
    if (gm_mouse.down) {
      tr.rotation.x += gm_mouse.movement.y * 2;
      tr.rotation.y -= gm_mouse.movement.x * 2;
    }
    gm3_project(&img, &tree, &tr, &scene);

    gm3_draw_image(&img, 0, 0, 1);

    gm3_image_reset(&img);
  } while (gm_yield());
  return 0;
}
