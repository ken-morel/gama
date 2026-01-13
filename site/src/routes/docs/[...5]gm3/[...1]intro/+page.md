# Handling 3d in gama

Want to add a 3d character? a cool boss or a cool 3d car?
Gama integrates a minimalistic 3d engine that allows you to load and display 3d objects in your game applying basic transformations.


## Meshes

A 3d object however you may load it, it is always represented by an object of type
<a href="/reference/structgm3_mesh.html">gm3Mesh</a>, which holds information about the object in a relatively compact form.
it may also contain material libraries(for colours), which on their turn may contain textures.

## gm3Image

Once you have a mesh(a 3d object), you can give gama specific informaiton
like light and rotation to project the object into an image constituted of
colored triangles.


## Reference

<a href="/reference/dir_6364b0f113d1e4eeaa335192002c4834.html">gama/3d folder reference</a>
