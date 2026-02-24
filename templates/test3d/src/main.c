#include <assets/gltf/head.glb.h>
#include <gama.h>
#include <gama/3d.h>
#include <gama/3d/viewer.h>

int main() {
  gm_init(500, 500, "skeleton gama application");
  gm_show_fps(1);

  gm3Mesh mesh = head_mesh();

  gm3Image image = gm3_image();

  gm3Transform transform = gm3_transform();
  transform.position.z = 10;

  gm3Scene scene = gm3_scene();
  scene.camera.focal = 1.5;

  do {
    gm3_model_view_bindings(&transform, 10, 5);
    // clear the previous image
    gm3_image_reset(&image);
    // project the mesh to an image(do all the maths)

    gm3_project(&image, &mesh, &transform, &scene);

    // draw the projection(draw the output)
    gm3_draw_image(&image, 0, 0, 1);
  } while (gm_yield());
  // destroy shapes
  return 0;
}
