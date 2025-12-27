#include <gama.h>
#include <gama/3d/obj.h>

int main() {
  gm_init(500, 500, "skeleton gama application");

  gm3ObjFile file;

  int code = gm3_load_obj(&file, "assets/obj/cube.obj");
  if (code != 0) {
    printf("object load exited with code %d\n", code);
    return 100;
  }

  if (file.n_objects != 1) {
    printf("File had %zu objects instead of one", file.n_objects);
    return 101;
  }

  gm3Mesh *mesh = &file.objects[0];

  printf("mesh 0 has %zu faces and %zu vertices", mesh->n_faces,
         mesh->n_vertices);

  // create shapes
  do {
    // do things here
  } while (gm_yield());
  // destroy shapes

  gm3_free_obj(&file);
  return 0;
}
