# The base

The base of the gama application is these simple three methods. You must specify them for your app to run.

When your app starts to when it stops, gama runs these few methods.

## `void init(App* app)`

This is the first method which gama calls. This method is intended to comfigure your just-created app, verify files or configuration before your app windows shows and even binding events directly to your app instead of a scene. You should not start loading fonts or creating objects here, it is strictly meant for initializing your application.

init() receives [The App] as only argument, since obviously it is meant to set it up.

Example:

```c
void init(App* app) {
  setAppTitle(app, "Hello world");
  setAppSize(app, 500, 500);
}
```

## `void create(App* app)`

Create shapes, scenes, fonts, and every thing else here. Create runs after the `init` and when your app opengl context has already been initialized, **This is why it is only safe to create fonts, images and shapres here**.

It is also in this that your are going to create and show [The scene](./scene.md).
