# API Reference

This is a comprehensive reference for the Gama game engine's API. 

## `app.h`

This file provides the core application management functionalities.

### `struct sApp`

Represents your game application.

**Fields**:

*   `char *title`: The title of the application window.
*   `unsigned int width`: The width of the application window in pixels.
*   `unsigned int height`: The height of the application window in pixels.
*   `void (*onclick)(struct sApp *, MouseClickEvent *)`: A function pointer that is called when a mouse click event occurs.
*   `void (*onkey)(struct sApp *, KeyEvent *)`: A function pointer that is called when a key event occurs.
*   `Scene *scene`: A pointer to the currently active scene.

### `App *GamaCreateApp()`

Creates and initializes a new `App` structure with default values.

**Returns**:

A pointer to the newly created `App`.

### `void showScene(App *app, Scene *s)`

Switches the currently active scene in the application. If there was a previous scene, it is destroyed.

**Parameters**:

*   `App *app`: A pointer to your application.
*   `Scene *s`: The new scene to be displayed.

### `void SetAppTitle(App *app, char *title)`

Sets the title of the application window.

**Parameters**:

*   `App *app`: A pointer to your application.
*   `char *title`: The new title for the window.

---

## `color.h`

This file defines the `Color` type and provides a set of predefined colors.

### `Color`

A `Color` is defined as an `unsigned int`. Colors are represented in RGBA format (e.g., `0xFF0000FF` for red).

### Color Constants

A wide range of color constants are defined, such as `RED`, `GREEN`, `BLUE`, `BLACK`, `WHITE`, `YELLOW`, etc.

### `void SetClearColor(Color col)`

Sets the background color that the screen is cleared to on each frame.

**Parameters**:

*   `Color col`: The color to clear the screen with.


---

## `drawshape.h`

Provides functions for drawing basic geometric shapes.

### `void drawRectangle(double x, double y, double width, double height, Color color)`

Draws a rectangle centered at `(x, y)`.

**Parameters**:

*   `double x`: The x-coordinate of the center of the rectangle.
*   `double y`: The y-coordinate of the center of the rectangle.
*   `double width`: The width of the rectangle.
*   `double height`: The height of the rectangle.
*   `Color color`: The color of the rectangle.

### `void drawCircle(double cx, double cy, double radius, Color color)`

Draws a circle centered at `(cx, cy)`.

**Parameters**:

*   `double cx`: The x-coordinate of the center of the circle.
*   `double cy`: The y-coordinate of the center of the circle.
*   `double radius`: The radius of the circle.
*   `Color color`: The color of the circle.

---

## `events.h`

Defines the structures for handling user input events.

### `struct MouseClickEvent`

Represents a mouse click event.

**Fields**:

*   `double x`: The x-coordinate of the mouse click (from -1.0 to 1.0).
*   `double y`: The y-coordinate of the mouse click (from -1.0 to 1.0).
*   `int button`: The mouse button that was clicked (e.g., `GLFW_MOUSE_BUTTON_LEFT`).
*   `int down`: 1 if the button was pressed, 0 if it was released.

### `enum Key`

An enumeration of all the keyboard keys that can be detected by Gama.

Examples: `KeyLeft`, `KeyRight`, `KeySpace`, `KeyEnter`, `KeyA`, `KeyB`, etc.

### `struct KeyEvent`

Represents a keyboard event.

**Fields**:

*   `Key key`: The key that was pressed or released.
---

## `image.h`

This file provides functions for loading and manipulating images.

### `struct Image`

Represents an image.

**Fields**:

*   `unsigned int width`: The width of the image in pixels.
*   `unsigned int height`: The height of the image in pixels.
*   `unsigned char *data`: A pointer to the raw pixel data of the image.
*   `unsigned int texture_id`: The OpenGL texture ID for the image.

### `Image *openImageFile(Image *img, const char *path)`

Loads an image from a file.

**Parameters**:

*   `Image *img`: A pointer to an `Image` struct to load the data into.
*   `const char *path`: The path to the image file.

**Returns**:

A pointer to the loaded `Image`, or `NULL` on failure.

### `Image *newImage()`

Allocates memory for a new `Image` struct.

**Returns**:

A pointer to the new `Image`.

### `Image *createImage(unsigned int width, unsigned int height)`

Creates a new blank `Image` with the specified dimensions.

**Parameters**:

*   `unsigned int width`: The width of the new image.
*   `unsigned int height`: The height of the new image.

**Returns**:

A pointer to the new `Image`.

### `Image *cropImage(const Image *img, unsigned int startx, unsigned int starty, unsigned int width, unsigned int height)`

Creates a new `Image` by cropping a section of an existing image.

**Parameters**:

*   `const Image *img`: The image to crop from.
*   `unsigned int startx`: The starting x-coordinate of the crop.
*   `unsigned int starty`: The starting y-coordinate of the crop.
*   `unsigned int width`: The width of the cropped image.
*   `unsigned int height`: The height of the cropped image.

**Returns**:

A pointer to the new cropped `Image`.

### `void drawImage(Image *image, double x, double y, double w, double h)`

Draws an image to the screen.

**Parameters**:

*   `Image *image`: The image to draw.
*   `double x`: The x-coordinate of the top-left corner.
*   `double y`: The y-coordinate of the top-left corner.
*   `double w`: The width to draw the image.
*   `double h`: The height to draw the image.

### `void freeImage(Image *image)`

Frees the memory used by an `Image`.

**Parameters**:

*   `Image *image`: The image to free.

---

## `scene.h`

This file provides the structure and functions for managing scenes.

### `struct sScene`

Represents a scene in the game.

**Fields**:

*   `SpriteList sprites`: A list of sprites in the scene.
*   `ShapeList shapes`: A list of shapes in the scene.
*   `Color background`: The background color of the scene.
*   `struct sApp *app`: A pointer to the application.
*   `void (*create)(struct sScene *)`: Function pointer called when the scene is created.
*   `void (*update)(struct sScene *, double)`: Function pointer called every frame to update the scene.
*   `void (*render)(struct sScene *)`: Function pointer called every frame to render the scene.
*   `void (*destroy)(struct sScene *)`: Function pointer called when the scene is destroyed.
*   `void (*onclick)(struct sScene *, MouseClickEvent *)`: Function pointer for mouse click events.
*   `void (*onkey)(struct sScene *, KeyEvent *)`: Function pointer for key events.

### `Scene *createScene(struct sApp *app)`

Creates a new, empty scene.

**Parameters**:

*   `struct sApp *app`: A pointer to the application.

**Returns**:

A pointer to the new `Scene`.

### `void addShapeToScene(Scene *scene, Shape *shape)`

Adds a shape to the scene.

**Parameters**:

*   `Scene *scene`: The scene to add the shape to.
*   `Shape *shape`: The shape to add.

### `void addSpriteToScene(Scene *scene, Sprite *sprite)`

Adds a sprite to the scene.

**Parameters**:

*   `Scene *scene`: The scene to add the sprite to.
*   `Sprite *sprite`: The sprite to add.

---

## `shape.h`

This file provides everything needed to create and manage shapes.

### `enum ShapeType`

An enum to represent the type of a shape.

**Values**:

*   `RectangleShape`
*   `CircleShape`

### `struct Shape`

Represents a geometric shape.

**Fields**:

*   `ShapeType type`: The type of the shape.
*   `Vector *pos`: The position and movement vector of the shape.
*   `Pos *size`: The size of the shape (for rectangles).
*   `Color color`: The color of the shape.
*   `double radius`: The radius of the shape (for circles).

### `Shape *createRectangle(Shape *s, Pos *pos, Pos *size, Color col)`

Initializes a `Shape` structure as a rectangle.

**Parameters**:

*   `Shape *s`: A pointer to the `Shape` to initialize.
*   `Pos *pos`: The position of the rectangle.
*   `Pos *size`: The size of the rectangle.
*   `Color col`: The color of the rectangle.

**Returns**:

A pointer to the initialized `Shape`.

### `Shape *createCircle(Shape *s, Pos *pos, double radius, Color col)`

Initializes a `Shape` structure as a circle.

**Parameters**:

*   `Shape *s`: A pointer to the `Shape` to initialize.
*   `Pos *pos`: The position of the circle.
*   `double radius`: The radius of the circle.
*   `Color col`: The color of the circle.

**Returns**:

A pointer to the initialized `Shape`.

### Shape Helpers

#### Physics and Position

*   `void setShapeVelocity(Shape *s, Pos *vel)`: Sets the velocity of a shape.
*   `Pos *getShapeVelocity(Shape *s)`: Gets the velocity of a shape.
*   `Pos *getShapePosition(Shape *s)`: Gets the position of a shape.
*   `void setShapePosition(Shape *s, Pos *pos)`: Sets the position of a shape.
*   `Pos *getShapeAcceleration(Shape *s)`: Gets the acceleration of a shape.
*   `void setShapeAcceleration(Shape *s, Pos *acc)`: Sets the acceleration of a shape.

#### Collision and Bouncing

*   `double shapeTop(Shape *s)`: Returns the y-coordinate of the top edge of a shape.
*   `double shapeBottom(Shape *s)`: Returns the y-coordinate of the bottom edge of a shape.
*   `double shapeLeft(Shape *s)`: Returns the x-coordinate of the left edge of a shape.
*   `double shapeRight(Shape *s)`: Returns the x-coordinate of the right edge of a shape.
*   `void bounceShape(Shape *s, double x, double y)`: Inverts the velocity of a shape on the x or y axis, simulating a bounce. A value of `0.0` for `x` or `y` means no bounce on that axis.

---

## `sprite.h`

This file provides everything needed to create and manage animated sprites.

### `struct Sprite`

Represents a sprite.

**Fields**:

*   `Vector *pos`: The position and movement vector of the sprite.
*   `Pos *size`: The size of the sprite.
*   `unsigned int length`: The number of frames in the sprite sheet.
*   `Image **images`: An array of images representing the frames of the sprite.
*   `float fps`: The speed of the animation in frames per second.
*   ... and other internal animation fields.

### `Sprite *createSprite(const char *path, unsigned int width, unsigned int height, Pos *pos, Pos *size)`

Creates a new sprite from a sprite sheet image.

**Parameters**:

*   `const char *path`: The path to the sprite sheet image.
*   `unsigned int width`: The width of a single frame in the sprite sheet.
*   `unsigned int height`: The height of a single frame in the sprite sheet.
*   `Pos *pos`: The initial position of the sprite.
*   `Pos *size`: The size to render the sprite.

**Returns**:

A pointer to the new `Sprite`.

### `void setSpriteAnimation(Sprite *s, unsigned int length, unsigned int *animation, unsigned int fps)`

Sets a custom animation sequence for a sprite.

**Parameters**:

*   `Sprite *s`: The sprite to animate.
*   `unsigned int length`: The number of frames in the animation sequence.
*   `unsigned int *animation`: An array of frame indices for the animation.
*   `unsigned int fps`: The speed of the animation in frames per second.

### Sprite Helpers

#### Physics and Position

*   `void setSpriteVelocity(Sprite *s, Pos *vel)`: Sets the velocity of a sprite.
*   `Pos *getSpriteVelocity(Sprite *s)`: Gets the velocity of a sprite.
*   `Pos *getSpritePosition(Sprite *s)`: Gets the position of a sprite.
*   `void setSpritePosition(Sprite *s, Pos *pos)`: Sets the position of a sprite.
*   `Pos *getSpriteAcceleration(Sprite *s)`: Gets the acceleration of a sprite.
*   `void setSpriteAcceleration(Sprite *s, Pos *acc)`: Sets the acceleration of a sprite.

#### Collision and Bouncing

*   `double spriteTop(Sprite *s)`: Returns the y-coordinate of the top edge of a sprite.
*   `double spriteBottom(Sprite *s)`: Returns the y-coordinate of the bottom edge of a sprite.
*   `double spriteLeft(Sprite *s)`: Returns the x-coordinate of the left edge of a sprite.
*   `double spriteRight(Sprite *s)`: Returns the x-coordinate of the right edge of a sprite.
*   `void bounceSprite(Sprite *s, double x, double y)`: Inverts the velocity of a sprite on the x or y axis.
*   `float bounceSpriteUnder(Sprite *s, double pos, double bounce, double maxSpeed)`: A specialized bounce function that simulates a bounce when a sprite hits a floor at a certain y-position.
*   `int rectsCollide(Pos *pa, Pos *sa, Pos *pb, Pos *sb)`: Checks if two rectangles collide.
*   `int spriteTouchesSprite(Sprite *a, Sprite *b)`: Checks if two sprites are colliding.

---

## `text.h`

This file provides functions for loading fonts and rendering text.

### `struct Font`

Represents a loaded font.

### `struct Text`

Represents a piece of text to be rendered.

**Fields**:

*   `char *text`: The text string to render.
*   `double fontsize`: The size of the font.
*   `Font *font`: The font to use.
*   `Vector *pos`: The position of the text.
*   `Color color`: The color of the text.

### `Font *loadFont(const char *url)`

Loads a font from a TTF file.

**Parameters**:

*   `const char *url`: The path to the font file.

**Returns**:

A pointer to the loaded `Font`.

### `Text *createText(const char *text, size_t length, Font *f, Pos *pos)`

Creates a new text object.

**Parameters**:

*   `const char *text`: The text to display.
*   `size_t length`: The length of the text.
*   `Font *f`: The font to use.
*   `Pos *pos`: The position of the text.

**Returns**:

A pointer to the new `Text` object.

### `void renderText(Text *t)`

Renders a text object to the screen.

**Parameters**:

*   `Text *t`: The text object to render.

### `void setText(Text *t, const char *txt, size_t length)`

Updates the text of a text object.

**Parameters**:

*   `Text *t`: The text object to update.
*   `const char *txt`: The new text.
*   `size_t length`: The length of the new text.

---

## `time.h`

Provides time-related functions.

### `double get_elapsed_seconds(void)`

Returns the number of seconds elapsed since the start of the application.

**Returns**:

The elapsed time in seconds.

---

## `vector.h`

Provides structures and functions for 2D vector math.

### `struct Pos`

Represents a 2D position or vector.

**Fields**:

*   `double x`: The x-component.
*   `double y`: The y-component.

### `Pos *at(double x, double y)`

Creates a new `Pos` object.

**Parameters**:

*   `double x`: The x-coordinate.
*   `double y`: The y-coordinate.

**Returns**:

A pointer to the new `Pos` object.

### `struct Vector`

Represents a physics vector with position, velocity, and acceleration.

**Fields**:

*   `Pos *pos`: The position.
*   `Pos *vel`: The velocity.
*   `Pos *acc`: The acceleration.

### `void updateVector(Vector *v, double theta)`

Updates the position and velocity of a vector based on its acceleration and the elapsed time.

**Parameters**:

*   `Vector *v`: The vector to update.
*   `double theta`: The elapsed time since the last update.