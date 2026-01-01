#define GM_SETUP

#include <gama.h>
#include <gama/3d.h>
#include <gama/debug.h>

#include <assets/obj/alien.obj.h>
gm3Transform transform;
gm3Scene scene;
gm3Mesh mesh;

int setup() {
  gm_init(500, 500, "gama cube application");
  gm_show_fps(1);
  gm_fullscreen(1);

  gm_background(GM_BLACK);

  mesh = alien_mesh();

  scene = gm3_scene();
  scene.light.position = (gm3Pos){0, 1, 0};
  scene.light.color = GM_WHITE;
  scene.camera.focal = 100;
  scene.camera.far = 150;
  scene.light.position = (gm3Pos){0, 5, 0};

  transform = gm3_transform();
  transform.position.z = 100;

  return 0;
}

int loop() {
  gm3Image img = gm3_image();
  double k = 0.1;
  if (gm_key('U'))
    transform.rotation.x -= k;
  else if (gm_key('D'))
    transform.rotation.x += k;

  if (gm_key('L'))
    transform.rotation.y -= k;
  else if (gm_key('R'))
    transform.rotation.y += k;

  if (gm_key('i'))
    transform.position.z -= k * 5;
  else if (gm_key('o'))
    transform.position.z += k * 5;

  gm3_image_clear(&img);

  gm3_project(&img, &mesh, &transform, &scene); // snap on the image

  char nt[100] = {0};
  gm3_draw_image(&img, 0, 0, 0.1); // draw the image
  snprintf(nt, sizeof(nt), "triangles: %zu", img.n_triangles);
  gmw_frame(0.9, -0.7, 0.4, 0.1);
  gm_draw_text(0.9, -0.7, nt, "", 0.07, GM_WHITE);
  // destroy shapes

  gm3_scene_free(&scene);
  return 0;
}
