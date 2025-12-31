#include "gama/color.h"
#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(500, 500, "gama cube application");
  gm_show_fps(1);
  gm_fullscreen(1);

  for (size_t i = 0; i < 5; i++) {
    gm_logo(0, 0, 1);
    gm_draw_text(0, -0.7, "Loading model...", "", 0.1, GM_WHITE);
    gm_yield();
  }
  gm_background(GM_BLACK);

  gm3Mesh mesh; // create an obj file

  // cube, bgirl, Alien Animal, tree
  printf("loading file...\n");
  int code = gm3_obj_load(&mesh, "assets/obj/tree.obj",
                          "assets/obj"); // load the obj file
  printf("loaded mesh with code %d\n", code);
  if (code < 0) {
    printf("Object load failed: %d", code);
    return code;
  } else if (code > 0) {
    printf("Object load notified: %d", code);
  }

  gmdn(mesh, mesh);

  gm3Scene scene = gm3_scene();
  scene.light.position = (gm3Pos){0, 1, 0};
  scene.light.color = GM_WHITE;

  scene.camera.focal = 1.5;

  scene.light.position = (gm3Pos){0, 0, 0};

  gm3Transform transform = gm3_transform();
  transform.position.z = 10;

  gm3Image img =
      gm3_image(); // the image where we snap the 3d object into a 2d image and
  // then draw

  gm3DrawImage.ignore_small_triangles = 0.000001;
  double dirty = 1;
  do {
    double k = gm_dt();
    if (gm_key('U')) {
      transform.rotation.x -= k;
      dirty = 1;
    } else if (gm_key('D')) {
      transform.rotation.x += k;
      dirty = 1;
    }

    if (gm_key('L')) {
      transform.rotation.y -= k;
      dirty = 1;
    } else if (gm_key('R')) {
      transform.rotation.y += k;
      dirty = 1;
    }

    if (gm_key('i')) {
      transform.position.z -= k * 5;
      dirty = 1;
    } else if (gm_key('o')) {
      transform.position.z += k * 5;
      dirty = 1;
    }

    if (dirty) {
      gm3_image_clear(&img);

      gm3_project(&mesh, &transform, &scene,
                  &img); // snap on the image
      dirty = 0;
    }
    char nt[100] = {0};
    sprintf(nt, "triangles: %zu", img.n_triangles);
    gm3_draw_image(&img, 0, 0); // draw the image
    gmw_frame(0.9, -0.9, 0.3, 0.1);
    gm_draw_text(0.9, -0.9, nt, "", 0.07, GM_WHITE);

  } while (gm_yield());
  // destroy shapes

  gm3_mesh_free(&mesh);
  gm3_scene_free(&scene);
  return 0;
}
