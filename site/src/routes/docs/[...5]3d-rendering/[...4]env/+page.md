# Creating a scene and environment

To render a 3d model, you need to give information to gama about things like
the lighting, the transformations on the objects, camera position and orientation, and more.

## The scene

The information about the scene is stored in the `gm3Scene` object, information
stored in the scene include:
- The lighting position, color and intensity.
- The camera position and viewport.

See <a href="/reference/scene_8h.html">gama/3d/scene.h reference</a>

```c
gm3Scene scene = gm3_scene();
scene.viewport
scene.light
scene.camera
```

### The scene viewport

This simply tells the size and ratio of the lens or window viewing your scene.
The default scene has a viewport of `(gmPos){2, 2}`, which should be satisfactory
in most cases. However, you can change it to fit your needs.

### The scene lighting

Information about the scene lighting is stored in the `gmLight` object, which
contains the position, ambient factor, color, direction and intensity of the light.



### The scene camera

Gama takes for convention and simplicity that in all scenes, the camera is placed at `(gmPos){0, 0, 0}`, 
the `gm3Scene` object also contains the camera focal length, which alters the perspective of the scene, 
if you give it a lower value, it will be like viewing the scene with smaller lens, and will make the scene
look even more distorted. It also contains the near and far clipping planes, which determine the range of the scene that is rendered, if something is further than `far` or closer than `near`, even just partly, it will not be displayed.


### An example scene

```c
gm3Scene myScene = gm3_scene(); // the default scene

myScene.light.color = GM_ORANGE; 

myScene.camera.focal = 1.5;
```
### The object transformation

The `gm3Translation` object stores transformation information about an object, such as its position, rotation, and scale.
exactly as stated.
