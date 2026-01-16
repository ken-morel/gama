# Projecting the object

View <a href="/reference/project_8h.html">gama/3d/project.h reference</a>

Once we got the object, scene and other information, we 
can draw on the image.
I case you have several objects to draw, you should draw them all in the same
image by consecutive `gm3_project` calls, and after drawing the image
you can use `gm3_image_reset` to clear the image, but without deleting
the allocated memory so that it can be reused.

```c
#include <gama.h>
#include <gama/3d.h>
#include <assets/obj/tree.obj.h>

int main() {
    const gm3Mesh tree = tree_mesh();
    
    gm3Transform transform = gm3_transform();
    transform.position.z = 20;
    
    gm3Scene scene = gm3_scene();
    scene.light.color = GM_GRAY;
    
    gm3Image image = gm3_image();
    
    do {
        gm3_project(&image, &tree, &transform, &scene);
        // OR you can use default transform and scene
        gm3_project(&image, &tree, NULL, NULL);
        
        // then you draw
        gm3_draw_image(&image, 0, 0, 1);
        
        gm3_image_reset(&image);
    } while(gm_yield());
    
    gm3_image_free(&image);
}
```
