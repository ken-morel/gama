# Building a Pong Game (AI generated)

> This tutorial is AI-generated

This tutorial will guide you through creating a classic Pong game from scratch using the Gama engine. This is a great project for beginners as it covers all the fundamental concepts of game development in a simple and easy-to-understand way.

## Prerequisites

Before you begin, make sure you have the following installed:

- A C compiler (like GCC)
- The `gama` command-line tool

## 1. Setting Up Your Project

First, let's create a new project. Open your terminal and run:

```bash
gama create mypong
```

This will create a new directory called `mypong` with a basic project structure.

## 2. The `main.c` File

Navigate into your new project directory and open the `src/main.c` file. This is the entry point for your game. You will see three main functions:

- `init(App *app)`: Called once when your application starts.
- `create(App *app)`: Called after the application window has been created.
- `shutdown(App *app)`: Called when the application is about to close.

Let's start by setting a title for our game window in the `init` function:

```c
// src/main.c
#include "../assets/gama/gama.h"

void init(App *app) {
    SetAppTitle(app, "Gama Pong");
}

void create(App *app) {}
void shutdown(App *app) {}
```

## 3. Setting up the Game Scene

For Pong, we only need one scene. We can set this up directly in our `main.c` file. We'll define the functions for our scene and then assign them in the `create` function.

```c
// src/main.c
#include "../assets/gama/gama.h"

// Forward declarations for our scene functions
void gameCreate(Scene *scene);
void gameUpdate(Scene *scene, double theta);
void gameRender(Scene *scene);
void gameKey(Scene *scene, KeyEvent *e);

void init(App *app) {
    SetAppTitle(app, "Gama Pong");
}

void create(App *app) {
    Scene *gameScene = createScene(app);
    gameScene->create = gameCreate;
    gameScene->update = gameUpdate;
    gameScene->render = gameRender;
    gameScene->onkey = gameKey;
    gameScene->background = BLACK;
    showScene(app, gameScene);
}

void shutdown(App *app) {}

// ... implementations of gameCreate, gameUpdate, etc. will go here ...
```

## 4. Creating the Game Objects

We need two paddles and a ball. These will be `Shape` objects. Let's declare them as global variables and initialize them in `gameCreate`.

```c
// ... after #include
Shape leftPaddle, rightPaddle, ball;

void gameCreate(Scene *scene) {
    // Left Paddle
    createRectangle(&leftPaddle, at(-0.9, 0), at(0.05, 0.3), WHITE);

    // Right Paddle
    createRectangle(&rightPaddle, at(0.9, 0), at(0.05, 0.3), WHITE);

    // Ball
    createCircle(&ball, at(0, 0), 0.025, WHITE);
    setShapeVelocity(&ball, at(0.5, 0.5)); // Give the ball some initial speed
}
```

### A Note on the Coordinate System

Gama uses a 2D Cartesian coordinate system where the center of the screen is `(0, 0)`. The x-axis ranges from -1.0 (left edge) to 1.0 (right edge), and the y-axis ranges from -1.0 (bottom edge) to 1.0 (top edge).

## 5. Rendering the Game

Now, let's draw our shapes in the `gameRender` function.

```c
void gameRender(Scene *scene) {
    renderShape(&leftPaddle);
    renderShape(&rightPaddle);
    renderShape(&ball);
}
```

If you run the game now (`gama run`), you should see the paddles and the ball, but nothing is moving yet.

## 6. Moving the Paddles

We need to handle keyboard input to move the paddles. We'll do this in the `gameKey` function.

```c
void gameKey(Scene *scene, KeyEvent *e) {
    double paddleSpeed = 1.0;
    if (e->key == KeyW) {
        setShapeVelocity(&leftPaddle, at(0, paddleSpeed));
    }
    if (e->key == KeyS) {
        setShapeVelocity(&leftPaddle, at(0, -paddleSpeed));
    }
    if (e->key == KeyUp) {
        setShapeVelocity(&rightPaddle, at(0, paddleSpeed));
    }
    if (e->key == KeyDown) {
        setShapeVelocity(&rightPaddle, at(0, -paddleSpeed));
    }
}
```

## 7. The Game Loop and Frame-Rate Independence

Now we need to update the positions of our objects in `gameUpdate`. This function is the heart of our game's logic and is called on every frame.

This function receives a `theta` parameter, which is the time in seconds that has passed since the last frame (also known as **delta time**). It is crucial to use `theta` in your movement calculations to ensure your game runs at the same speed on all computers, regardless of their frame rate. This is called **frame-rate independence**.

```c
void gameUpdate(Scene *scene, double theta) {
    updateShape(&leftPaddle, theta);
    updateShape(&rightPaddle, theta);
    updateShape(&ball, theta);

    // Stop paddles at the window edges
    if (shapeTop(&leftPaddle) > 1.0 || shapeBottom(&leftPaddle) < -1.0) {
        setShapeVelocity(&leftPaddle, at(0, 0));
    }
    if (shapeTop(&rightPaddle) > 1.0 || shapeBottom(&rightPaddle) < -1.0) {
        setShapeVelocity(&rightPaddle, at(0, 0));
    }
}
```

Run the game now, and you should be able to move the paddles!

## 8. Collision and Bouncing

Let's make the ball bounce off the walls and the paddles. The `bounceShape` function is perfect for this. It works by inverting the velocity of a shape on a given axis. For example, `bounceShape(&ball, 0, 1.0)` will not change the x-velocity but will invert the y-velocity, making the ball bounce vertically.

```c
void gameUpdate(Scene *scene, double theta) {
    // ... (previous code from gameUpdate)

    // Ball bouncing off top and bottom walls
    if (shapeTop(&ball) > 1.0 || shapeBottom(&ball) < -1.0) {
        bounceShape(&ball, 0, 1.0);
    }

    // Ball bouncing off paddles
    if (rectsCollide(getShapePosition(&ball), at(0.05, 0.05), getShapePosition(&leftPaddle), leftPaddle.size) ||
        rectsCollide(getShapePosition(&ball), at(0.05, 0.05), getShapePosition(&rightPaddle), rightPaddle.size)) {
        bounceShape(&ball, 1.0, 0);
    }

    // Scoring and resetting the ball
    if (shapeLeft(&ball) < -1.0 || shapeRight(&ball) > 1.0) {
        setShapePosition(&ball, at(0, 0));
        setShapeVelocity(&ball, at(0.5, 0.5)); // Reset ball speed
    }
}
```

## 9. Adding Scoring

Let's add scoring. We'll need a font and two text objects.

```c
// ... after shape declarations
Font *font;
Text *leftScoreText, *rightScoreText;
int leftScore = 0, rightScore = 0;

void gameCreate(Scene *scene) {
    // ... (paddle and ball creation)

    font = loadFont("assets/fonts/Ubuntu-R.ttf");
    leftScoreText = createTextNulled("0", font, at(-0.1, 0.8));
    rightScoreText = createTextNulled("0", font, at(0.1, 0.8));
}

void gameRender(Scene *scene) {
    // ... (render shapes)
    renderText(leftScoreText);
    renderText(rightScoreText);
}

void updateScore() {
    char scoreStr[10];
    sprintf(scoreStr, "%d", leftScore);
    setTextNulled(leftScoreText, scoreStr);
    sprintf(scoreStr, "%d", rightScore);
    setTextNulled(rightScoreText, scoreStr);
}

// In gameUpdate, modify the scoring logic:
// ...
    if (shapeLeft(&ball) < -1.0) { // Right player scores
        rightScore++;
        updateScore();
        setShapePosition(&ball, at(0, 0));
    }
    if (shapeRight(&ball) > 1.0) { // Left player scores
        leftScore++;
        updateScore();
        setShapePosition(&ball, at(0, 0));
    }
// ...
```

## 10. Game Over and Restarting

Let's add a simple game over condition. The first player to reach 5 points wins.

```c
// ... after score variables
Text *gameOverText;
int gameOver = 0; // 0 for playing, 1 for game over

void gameCreate(Scene *scene) {
    // ...
    gameOverText = createTextNulled("", font, at(0, 0));
    gameOverText->fontsize = 0.2;
}

void resetGame() {
    leftScore = 0;
    rightScore = 0;
    updateScore();
    setShapePosition(&ball, at(0, 0));
    setShapeVelocity(&ball, at(0.5, 0.5));
    setTextNulled(gameOverText, "");
    gameOver = 0;
}

// In gameKey, add a restart key:
void gameKey(Scene *scene, KeyEvent *e) {
    // ... (paddle movement)
    if (e->key == KeyR && gameOver) {
        resetGame();
    }
}

// In gameUpdate, check for game over:
void gameUpdate(Scene *scene, double theta) {
    if (gameOver) return; // Don't update the game if it's over

    // ... (rest of the update logic)

    if (leftScore >= 5) {
        setTextNulled(gameOverText, "Left Player Wins!\nPress R to Restart");
        gameOver = 1;
    }
    if (rightScore >= 5) {
        setTextNulled(gameOverText, "Right Player Wins!\nPress R to Restart");
        gameOver = 1;
    }
}

// In gameRender, draw the game over text:
void gameRender(Scene *scene) {
    // ... (render shapes and scores)
    if (gameOver) {
        renderText(gameOverText);
    }
}
```

## 11. Conclusion

You now have a complete, working Pong game with a scoring and game over system! This tutorial has covered the basics of creating a game in Gama. From here, you can try to add more features, like a main menu, sound effects, or increasing ball speed over time.

Happy coding!
