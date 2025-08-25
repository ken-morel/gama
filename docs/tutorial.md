# Tutorial: Building Your First Game

This tutorial will guide you through the process of creating a simple 2D platformer game using the Gama engine. We will build a game where you control a character named "Robi" who has to jump over cactuses. This will teach you the basics of creating an application, handling sprites, managing scenes, and responding to user input.

## Prerequisites

Before you begin, make sure you have the following installed:

*   A C compiler (like GCC)
*   The `gama` command-line tool

## 1. Setting Up Your Project

First, let's create a new project. Open your terminal and run:

```bash
gama create mygame
```

This will create a new directory called `mygame` with a basic project structure.

## 2. Understanding the `main.c` File

Navigate into your new project directory and open the `src/main.c` file. This is the entry point for your game. You will see three main functions:

*   `init(App *app)`: This function is called once at the very beginning when your application starts. It's the perfect place to set up initial game state, like setting the window title.
*   `create(App *app)`: This function is called after the application window has been created. You should use it to load assets like fonts and sprites, and to set up your initial game scene.
*   `shutdown(App *app)`: This function is called once when the application is about to close. You can use it to clean up any resources.

## 3. Your First Window

Let's change the title of our game window. In the `init` function in `src/main.c`, add the following line:

```c
#include "../assets/gama/gama.h"

void init(App *app) {
    SetAppTitle(app, "Robi's Adventure");
}

void create(App *app) {}
void shutdown(App *app) {}
```

Now, you can build and run your game:

```bash
gama run
```

You should see a window appear with the title "Robi's Adventure".

## 4. Introduction to Scenes

In Gama, your game is organized into **Scenes**. A scene is like a single screen in your game, for example, the main menu, the game itself, or the game over screen. Each scene has its own set of functions to manage its state:

*   `create`: Called when the scene is first shown.
*   `update`: Called on every frame to update the game logic.
*   `render`: Called on every frame to draw everything in the scene.
*   `onkey`: Called when a key is pressed.
*   `onclick`: Called when the mouse is clicked.
*   `destroy`: Called when the scene is about to be removed.

We will create three scenes for our game:

1.  `welcomeScene`: To show a welcome message.
2.  `robiScene`: The main game scene.
3.  `gameoverScene`: To show when the game is over.

## 5. Creating the Welcome Scene

Let's create a new header file `src/welcome.h` to manage our welcome scene.

```c
// src/welcome.h
#include "../../assets/gama/gama.h"

Text *welcomeText;
Font *font;

void welcomeCreate(Scene *scene) {
  font = loadFont("assets/fonts/Ubuntu-R.ttf");
  welcomeText = createTextNulled("Press P to Play", font, at(0, 0));
  welcomeText->color = DARKBLUE;
}

void welcomeRender(Scene *scene) {
  renderText(welcomeText);
}

void welcomeKey(Scene *scene, KeyEvent *e) {
  if (e->key == KeyP) {
    // We will switch to the game scene later
  }
}

Scene *createWelcomeScene(App *app) {
  Scene *welcome = createScene(app);
  welcome->create = welcomeCreate;
  welcome->render = welcomeRender;
  welcome->onkey = welcomeKey;
  welcome->background = CYAN;
  return welcome;
}
```

Now, in `src/main.c`, let's create this scene and show it.

```c
// src/main.c
#include "defs.h"
#include "welcome.h"

Scene *welcomeScene;

void init(App *app) {
    SetAppTitle(app, "Robi's Adventure");
    welcomeScene = createWelcomeScene(app);
}

void create(App *app) {
    showScene(app, welcomeScene);
}

void shutdown(App *app) {}
```

We also need a `defs.h` file to share declarations between files.

```c
// src/defs.h
#include "../../assets/gama/gama.h"

```

Now if you run the game, you should see the welcome message.

This is the basic structure. In the next steps, we will create the Robi character, add obstacles, and handle the game logic.
