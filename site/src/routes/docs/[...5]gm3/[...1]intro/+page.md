# Handling 3d in gama

Want to add a 3d character? a cool boss or a cool 3d car?
Gama integrates a minimalistic 3d engine that allows you to load and display 3d objects in your game applying basic transformations.


## Meshes

A 3d object however you may load it, it is always represented by an object of type
`gm3Mesh`, which holds information about the object in a relatively compact form.
it may also contain material libraries(for colours), which on their turn may contain textures.
