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

  gm3ObjFile file; // create an obj file

  int code = gm3_load_obj(&file, "assets/obj/Bowl.obj"); // load the obj file

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
