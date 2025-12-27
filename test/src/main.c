#include "gama/3d/image.h"
#include "gama/3d/scene.h"
#include "gama/3d/transform.h"
#include "gama/gama.h"
#include "gama/image.h"
#include <gama.h>
#include <gama/3d.h>

int main() {
  gm_init(500, 500, "skeleton gama application");
  gm_show_fps(1);

  gm3ObjFile file;

  int code = gm3_load_obj(&file, "assets/obj/cube.obj");
  if (code != 0) {
    printf("object load exited with code %d\n", code);
    return 100;
  }

  if (file.n_objects != 1) {
    printf("File had %zu objects instead of one\n", file.n_objects);
    return 101;
  }

  gm3Mesh *mesh = &file.objects[0];

  printf("mesh 0 has %zu faces and %zu vertices\n", mesh->n_faces,
         mesh->n_vertices);

  gm3Scene scene = gm3_scene();

  gm3_scene_create(&scene, 2, 2);

  gm3Transform transform = gm3_transform();
  transform.position.z = 5;

  gm3Image img;

  gm3_project(mesh, &transform, &scene, &img);

  printf("outputs has %zu triangles\n", img.n_triangles);
  printf("n colors: %zu\n", img.n_colors);

  // create shapes
  do {
    // do things here
    transform.rotation.y += gm_dt();
    gm3_project(mesh, &transform, &scene, &img);
    gm3_draw_image(&img, -1, -1);
  } while (gm_yield());
  // destroy shapes

  gm3_free_obj(&file);
  gm3_free_scene(&scene);
  return 0;
}
