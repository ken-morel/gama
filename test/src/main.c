#include "gama/3d/obj.h"
#define GM_SETUP

#include <gama.h>
#include <gama/3d.h>
#include <gama/debug.h>

// #include <assets/obj/alien2k.obj.h>
gm3Transform transform;
gm3Scene scene;
gm3Mesh mesh;

gm3Image img;

int setup() {
  gm_init(500, 500, "gama cube application");
  gm_show_fps(1);
  gm_fullscreen(1);

  gm_background(GM_BLACK);

  // mesh = alien2k_mesh();
  gm3_obj_load(&mesh, "assets/obj/girl/girl OBJ.obj", "assets/obj/girl");

  scene = gm3_scene();
  scene.light.position = (gm3Pos){0, 1, 0};
  scene.light.color = GM_WHITE;
  scene.camera.focal = 100;
  scene.camera.far = 150;
  scene.light.position = (gm3Pos){0, 5, 0};

  transform = gm3_transform();
  transform.position.z = 10;

  img = gm3_image();
  return 0;
}

int loop() {
  if (gm_mouse.down) {
    transform.rotation.y -= gm_mouse.movement.x * 10;
    transform.rotation.x -= gm_mouse.movement.y * 10;
  }
  if (gm_key('i'))
    transform.position.z -= 0.2;
  else if (gm_key('o'))
    transform.position.z += 0.2;

  gm3_project(&img, &mesh, &transform, &scene); // snap on the image

  char nt[100] = {0};
  gm3_draw_image(&img, 0, 0, 0.1); // draw the image
  snprintf(nt, sizeof(nt), "triangles: %zu", img.n_triangles);
  gmw_frame(0.9, -0.7, 0.4, 0.1);
  gm_draw_text(0.9, -0.7, nt, "", 0.07, GM_WHITE);

  gm3_image_reset(&img);
  return 0;
}

char *bye() {
  gm3_image_free(&img);
  gm3_scene_free(&scene);

  return "bye world";
}
