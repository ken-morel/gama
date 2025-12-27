#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(500, 500, "gama cube application");
  gm_show_fps(1);
  gm_fullscreen(1);
  gm_background(GM_DARKGREY);

  gm3ObjFile file; // create an obj file

  int code = gm3_load_obj(&file, "assets/obj/tree.obj"); // load the obj file

  printf("obj file has %zu objects", file.n_objects);

  gm3Scene scene = gm3_scene();

  gm3Transform transform = gm3_transform();
  double scale = 0.5;
  transform.scale.x = scale;
  transform.scale.y = scale;
  transform.scale.z = scale;
  transform.position.z = 15;

  gm3Image img =
      gm3_image(); // the image where we snap the 3d object into a 2d image and
  // then draw
  gm3DrawImage.ignore_colors = 1;
  gm3DrawImage.ignored_colors[0] = GM_BLACK;

  do {
    gm3_image_clear(&img);
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
    }
    gm3_draw_image(&img, -1, -1); // draw the image
  } while (gm_yield());
  // destroy shapes

  gm3_free_obj(&file);
  gm3_free_scene(&scene);
  return 0;
}
