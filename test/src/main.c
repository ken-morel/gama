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
  gm_background(GM_DARKGREY);

  gm3Mesh mesh; // create an obj file

  // cube, bgirl, Alien Animal, tree
  printf("loading file...\n");
  int code = gm3_obj_load(&mesh, "assets/obj/triangle.obj",
                          "assets/obj"); // load the obj file
  printf("loaded mesh with code %d\n", code);
  if (code < 0) {
    printf("Object load failed: %d", code);
    return code;
  } else if (code > 0) {
    printf("Object load notified: %d", code);
  }
  printf(" - %zu\n", mesh.faces[0].vertices[0]);
  printf(" - %zu\n", mesh.faces[0].vertices[1]);
  printf(" - %zu\n", mesh.faces[0].vertices[2]);
  printf("file loaded with %zu faces\n", mesh.n_faces);
  printf("file loaded with %zu vertices\n", mesh.n_vertices);
  printf("file loaded with %zu normals\n", mesh.n_normals);
  printf("file has %zu textures\n", mesh.n_texs);

  gm3Scene scene = gm3_scene();
  scene.far = 20;
  scene.light.position.z = -10;
  scene.light.color = GM_ORANGE;

  gm3Transform transform = gm3_transform();
  double scale = 0.5;
  transform.scale.x = scale;
  transform.scale.y = scale;
  transform.scale.z = scale;
  transform.position.z = 15;

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
      printf("Image has %zu triangles\n", img.n_triangles);
      dirty = 0;
    }
    gm3_draw_image(&img, -1, -1); // draw the image
  } while (gm_yield());
  // destroy shapes

  gm3_mesh_free(&mesh);
  gm3_scene_free(&scene);
  return 0;
}
