# How-to: Create a Main Menu

Every game needs a main menu. This guide will show you how to create a simple main menu scene with options to start the game or exit.

## 1. The Concept: Scene-Based Menus

The easiest way to manage a main menu is to treat it as just another scene. Your game will have at least two scenes:

*   `MainMenuScene`: Displays the menu options.
*   `GameScene`: The actual game.

You will start the application by showing the `MainMenuScene`. When the player chooses to start the game, you will switch to the `GameScene`.

## 2. Creating the Menu Scene

Let's create a new header file `src/main_menu.h` for our menu scene.

```c
// src/main_menu.h
#include "../assets/gama/gama.h"

Text *titleText, *playText, *exitText;
Font *menuFont;

void menuCreate(Scene *scene) {
    menuFont = loadFont("assets/fonts/Ubuntu-R.ttf");
    titleText = createTextNulled("PONG", menuFont, at(0, 0.5));
    titleText->fontsize = 0.5;

    playText = createTextNulled("Play", menuFont, at(0, 0));
    playText->fontsize = 0.2;

    exitText = createTextNulled("Exit", menuFont, at(0, -0.3));
    exitText->fontsize = 0.2;
}

void menuRender(Scene *scene) {
    renderText(titleText);
    renderText(playText);
    renderText(exitText);
}

void menuKey(Scene *scene, KeyEvent *e) {
    if (e->key == KeyP) { // 'P' for Play
        // Code to switch to the game scene will go here
    }
    if (e->key == KeyEscape) { // 'Escape' to Exit
        // Code to exit the game will go here
    }
}

Scene *createMainMenuScene(App *app) {
    Scene *menu = createScene(app);
    menu->create = menuCreate;
    menu->render = menuRender;
    menu->onkey = menuKey;
    menu->background = DARKGRAY;
    return menu;
}
```

## 3. Integrating with Your Game

Now, let's modify the `main.c` from our Pong tutorial to use this main menu.

```c
// src/main.c
#include "../assets/gama/gama.h"
#include "main_menu.h" // Include the new menu header

// ... (declarations for the Pong game scene)

Scene *mainMenuScene;
Scene *gameScene;

void init(App *app) {
    SetAppTitle(app, "Gama Pong");
    mainMenuScene = createMainMenuScene(app);

    // We still create the game scene, but we don't show it yet
    gameScene = createScene(app);
    gameScene->create = gameCreate;
    gameScene->update = gameUpdate;
    gameScene->render = gameRender;
    gameScene->onkey = gameKey;
    gameScene->background = BLACK;
}

void create(App *app) {
    // Start by showing the main menu
    showScene(app, mainMenuScene);
}

// ... (rest of the Pong game code)
```

## 4. Switching Scenes

Finally, we need to implement the scene switching logic in `menuKey` in `src/main_menu.h`.

```c
// In src/main_menu.h

// We need access to the gameScene, so we need to declare it
extern Scene *gameScene;

void menuKey(Scene *scene, KeyEvent *e) {
    if (e->key == KeyP) { // 'P' for Play
        showScene(scene->app, gameScene);
    }
    if (e->key == KeyEscape) { // 'Escape' to Exit
        // To exit the game, you can call exit(0) or 
        // use a platform-specific way to close the window.
        // For now, we will just print a message.
        printf("Exit selected. Closing game.\n");
        exit(0);
    }
}
```

And in `src/main.c`, you need to make `gameScene` available to other files:

```c
// In src/main.c, before init()
Scene *gameScene; // Remove the `static` keyword if it's there
```

Now, when you run your game, you will be greeted with the main menu. Pressing `P` will start the game, and `Escape` will exit.
