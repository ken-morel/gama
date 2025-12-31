#include <gama.h>
#include <gama/3d.h>
#include <gama/debug.h>

#include <assets/obj/tree.obj.h>

char nt[100] = {0};

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

  gm3Mesh mesh = tree_mesh();

  // cube, bgirl, Alien Animal, tree
  printf("loading file...\n");

  gmdn(mesh, mesh);

  gm3Scene scene = gm3_scene();
  scene.light.position = (gm3Pos){0, 1, 0};
  scene.light.color = GM_WHITE;

  scene.camera.focal = 10;

  scene.light.position = (gm3Pos){0, 5, 0};

  gm3Transform transform = gm3_transform();
  transform.position.z = 10;

  gm3Image img =
      gm3_image(); // the image where we snap the 3d object into a 2d image and
  // then draw
  double dirty = 1;
  do {
    double k = gm_dt();
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
    dirty = 0;

    gm3_draw_image(&img, 0, 0, 0.1); // draw the image
    snprintf(nt, sizeof(nt), "triangles: %zu", img.n_triangles);
    gmw_frame(0.9, -0.7, 0.4, 0.1);
    gm_draw_text(0.9, -0.7, nt, "", 0.07, GM_WHITE);
  } while (gm_yield());
  // destroy shapes

  gm3_scene_free(&scene);
  return 0;
}
