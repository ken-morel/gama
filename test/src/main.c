#include "gama/3d/transform.h"
#include <assets/gltf/cube.glb.h>
#include <gama.h>
#include <gama/3d.h>
#include <gama/debug.h>

int main() {
  gm_init(0, 0, "Hello world");
  gm_show_fps(1);

  gm_fullscreen(1);

  gm3Mesh mesh = cube_mesh();

  gmdn(pos3, mesh.faces[0].normal);
  printf("%zu,", mesh.faces[0].vertices[0]);
  printf("%zu,", mesh.faces[0].vertices[1]);
  printf("%zu\n", mesh.faces[0].vertices[2]);

  gmdn(pos3, mesh.vertices[0]);
  gmdn(pos3, mesh.vertices[1]);
  gmdn(pos3, mesh.vertices[2]);

  gm3Transform trans = gm3_transform();
  trans.position.z = 3;

  gm_background(0x333355FF);

  gm3Image img = gm3_image();

  do {
    gm3_project(&img, &mesh, NULL, NULL);

    gm3_draw_image(&img, 0, 0, 1);

    gm3_image_reset(&img);
  } while (gm_yield());
  return 0;
}
