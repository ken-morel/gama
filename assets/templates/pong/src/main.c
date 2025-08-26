// Copyright © 2025 Mr.DCT(VERLA BERINYUY)
#include "../gama/gama.h"

// Forward declarations for our scene functions
void gameCreate(Scene *scene);
void gameUpdate(Scene *scene, double theta);
void gameRender(Scene *scene);
void gameKey(Scene *scene, KeyEvent *e);
void updateScore();
void resetGame();

Shape leftPaddle, rightPaddle, ball;
Font *font;
Text *leftScoreText, *rightScoreText;
int leftScore = 0, rightScore = 0;
Text *gameOverText;
int gameOver = 0; // 0 for playing, 1 for game over

void init(App *app) { SetAppTitle(app, "Gama Pong"); }

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

void resetGame() {
  leftScore = 0;
  rightScore = 0;
  updateScore();
  setShapePosition(&ball, at(0, 0));
  setShapeVelocity(&ball, at(0.5, 0.5));
  setText(gameOverText, "");
  gameOver = 0;
}

void gameCreate(Scene *scene) {
  // Left Paddle
  createRectangle(&leftPaddle, at(-0.9, 0), at(0.05, 0.3), WHITE);

  // Right Paddle
  createRectangle(&rightPaddle, at(0.9, 0), at(0.05, 0.3), WHITE);

  // Ball
  createCircle(&ball, at(0, 0), 0.025, WHITE);
  setShapeVelocity(&ball, at(0.5, 0.5)); // Give the ball some initial speed

  font = loadFont("assets/fonts/Ubuntu-R.ttf");
  leftScoreText = createText("0", 0.1, font, at(-0.1, 0.8));
  rightScoreText = createText("0", 0.1, font, 0.1, at(0.1, 0.8));
  gameOverText = createText("", 0.2, font, at(0, 0));
}

void gameRender(Scene *scene) {
  renderShape(&leftPaddle);
  renderShape(&rightPaddle);
  renderShape(&ball);
  renderText(leftScoreText);
  renderText(rightScoreText);
  if (gameOver) {
    renderText(gameOverText);
  }
}

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
  if (e->key == KeyR && gameOver) {
    resetGame();
  }
}

void updateScore() {
  char scoreStr[10];
  sprintf(scoreStr, "%d", leftScore);
  setText(leftScoreText, scoreStr);
  sprintf(scoreStr, "%d", rightScore);
  setText(rightScoreText, scoreStr);
}

void gameUpdate(Scene *scene, double theta) {
  if (gameOver)
    return;

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

  // Ball bouncing off top and bottom walls
  if (shapeTop(&ball) > 1.0 || shapeBottom(&ball) < -1.0) {
    bounceShape(&ball, 0, 1.0);
  }

  // Ball bouncing off paddles
  if (rectsCollide(getShapePosition(&ball), at(0.05, 0.05),
                   getShapePosition(&leftPaddle), leftPaddle.size) ||
      rectsCollide(getShapePosition(&ball), at(0.05, 0.05),
                   getShapePosition(&rightPaddle), rightPaddle.size)) {
    bounceShape(&ball, 1.0, 0);
  }

  // Scoring and resetting the ball
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

  if (leftScore >= 5) {
    setText(gameOverText, "Left Player Wins!\nPress R to Restart");
    gameOver = 1;
  }
  if (rightScore >= 5) {
    setText(gameOverText, "Right Player Wins!\nPress R to Restart");
    gameOver = 1;
  }
}
