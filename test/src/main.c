#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(500, 500, "gama cube application");
  gm_show_fps(1);
  gm_fullscreen(1);

  gm3ObjFile file; // create an obj file

  int code = gm3_load_obj(&file, "assets/obj/test.obj"); // load the obj file

  for (size_t i = 0; i < file.n_objects; i++) {
  }
  printf("obj file has %zu objects", file.n_objects);

  gm3Scene scene = gm3_scene();
  scene.light.color = GM_ORANGE;

  gm3Transform transform = gm3_transform();
  transform.position.z = 2;

  gm3Image img; // the image where we snap the 3d object into a 2d image and
  // then draw

  do {
    if (gm_mouse.down) {
      transform.rotation.y -= gm_mouse.movement.x * 2;
      transform.rotation.x -= gm_mouse.movement.y * 2;
    }
    for (size_t i = 0; i < file.n_objects;
         i++) { // draw each of the objects in the file
      gm3_project(&file.objects[i], &transform, &scene,
                  &img); // snap on the image
      for (size_t i = 0; i < img.n_colors; i++)
        img.colors[i] = gm_set_alpha(img.colors[i], 255);
      gm3_draw_image(&img, -1, -1); // draw the image
    }
  } while (gm_yield());
  // destroy shapes

  gm3_free_obj(&file);
  gm3_free_scene(&scene);
  return 0;
}
