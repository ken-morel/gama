#include "gama/color.h"
#include "gama/key.h"
#include <gama.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// the board is a square, with only a side
#define board_side 10
#define snake_max_size (board_side * board_side)

typedef struct {
  int x;
  int y;
} coordonate;

int board[board_side][board_side] = {0};
// the board:
// 0 -> empty
// 1 -> snake
// 2 -> food

coordonate snake[snake_max_size] = {0};
int snake_length = 0;
char snake_direction = 'U';
// the snake's moving direction
// U -> up
// D -> down
// L -> left
// R -> right

coordonate food;

void initialize_board() {
  // initialize the snake with one block:
  snake_length = 1;
  snake[0] = (coordonate){1, 1};
  // initialize the food
  food = (coordonate){board_side / 2, board_side / 2};
}

void draw_board() {
  const double block_seperation_ratio = 0.1;
  const double block_side_including_space = 2.0 / (double)board_side;
  const double block_space =
      block_side_including_space * block_seperation_ratio;
  const double block_side = block_side_including_space - (block_space * 2);

  gmRect rect = gmrect(block_space, block_space, block_side, block_side);

  for (size_t x = 0; x < board_side; x++) {
    for (size_t y = 0; y < board_side; y++) {
      printf("%lf\t", block_space);
      rect.pos.x =
          block_side / 2 + block_space + (x * block_side_including_space) - 1;
      rect.pos.y =
          block_side / 2 + block_space + (y * block_side_including_space) - 1;

      switch (board[x][y]) {
      case 0: // empty
        gm_rect(rect, GM_DARKGRAY);
        break;
      case 1: // snake
        gm_rect(rect, GM_BISQUE);
        break;
      case 2:
        gm_rect(rect, GM_RED);
        break;
      default: // we made an error
        gm_rect(rect, GM_BLACK);
        break;
      }
    }
  }
}

void clear_board() {
  // set all of board to 0
  memset(board, 0, sizeof(board));
}

void draw_things_on_board() {
  // draw food
  board[food.x][food.y] = 2;
  board[food.x][food.y] = 2;

  for (size_t i = 0; i < snake_length; i++) {
    coordonate square = snake[i];
    board[square.x][square.y] = 1;
  }
}

void eat_food() {
  if (snake_length < snake_max_size)
    snake_length += 1;
  int food_on_something_else = 0;
  do {
    food.x = rand() % board_side;
    food.y = rand() % board_side;
    for (size_t i = 0; i < snake_length; i++)
      if (snake[i].x == food.x && snake[i].y == food.y)
        food_on_something_else = 1;
  } while (food_on_something_else);
}

void update_things_on_board() {
  // test if the snake eats the food
  if (snake[0].x == food.x && snake[0].y == food.y)
    eat_food();

  // every part of the snake takes the position
  // of the preceding part(even the just added one)
  for (size_t i = snake_length - 1; i > 0; i--)
    snake[i] = snake[i - 1];
  switch (snake_direction) {
  case 'U':
    snake[0].y += 1;
    break;
  case 'D':
    snake[0].y -= 1;
    break;
  case 'L':
    snake[0].x -= 1;
    break;
  case 'R':
    snake[0].x += 1;
  }
}

int main() {
  srand(time(NULL));

  double update_seconds = 0.5; // move every 5 seconds
  double time_since_last_update = 0;
  initialize_board();
  gm_init(500, 500, "gama snake game");
  do {
    time_since_last_update += gm_dt();
    if (time_since_last_update > update_seconds) {
      update_things_on_board();
      time_since_last_update = 0;
    }
    clear_board();
    draw_things_on_board();
    draw_board();
    char directions[4] = "UDLR";
    for (size_t i = 0; i < 4; i++)
      if (gm_key(directions[i]))
        snake_direction = directions[i];

  } while (gm_yield());
  return 0;
}
