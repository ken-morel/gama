#include "animate.h"
#include "draw.h"
#include <gama.h>
#include <stdio.h>

int gameover = 0;

void pong_scene() {
  int score = 1;
  char score_text[20];

  gmSystem sys = gm_system_create();

  gmBody ball_body = gm_circle_body(100, 0, 0, 0.2);
  gm_system_push(&sys, &ball_body);

  gmBody walls[2] = {
      gm_rectangle_body(0, 0, -1, 2, 0),
      gm_rectangle_body(0, 0, 1, 2, 0),
  };
  gm_system_push_array(&sys, 2, walls);

  gmBody paddles[2] = {
      gm_rectangle_body(0, -1, 0, 0.2, 0.7),
      gm_rectangle_body(0, 1, 0, 0.2, 0.7),
  };
  gm_system_push_array(&sys, 2, paddles);

  gmBody goal_body = gm_circle_body(50, 0, -0.5, 0.05);
  gm_system_push(&sys, &goal_body);

  double paddlev = 0;

  do {
    double dt = gm_dt();

    gm_speed_anim(&ball_body, 1.3, gm_anim_spring, dt, 0.5);
    gm_speed_anim(&goal_body, 0.5, gm_anim_spring, dt, 2);

    gm_body_bound_reflect(&goal_body, -1.05, 1.05, -1.05, 1.05);
    if (0b1100 &&
        gm_body_bound_reflect(&ball_body, -1.2, 1.2, -1.2, 1.2) != 0b0000)
      score--;

    if (gm_system_get_collision(&sys, &ball_body, &goal_body) != NULL) {
      score++;
    } else if (score < -1) {
      gameover = 1;
      return;
    }

    if (gm_key('U'))
      paddlev = -1;
    else if (gm_key('D'))
      paddlev = 1;
    else
      paddlev *= 0.90; // decrease smoothly

    gm_anim_linear(&paddles[0].velocity.y, paddlev, dt, 0.3);
    gm_anim_linear(&paddles[1].velocity.y, paddlev, dt, 0.3);

    sprintf(score_text, "Score: %d", score);

    gm_system_update(&sys);

    gm_draw_circle_body(&ball_body, GM_BISQUE);
    gm_draw_rect_bodies(paddles, 2, GM_DARKGOLDENROD);
    gm_draw_circle_body(&goal_body, GM_GREEN);
    gm_draw_text(0, 0.9, score_text, "", 0.1, GM_BLACK);

    if (paddles[0].position.y < -1.2)
      gm_draw_rectangle(0, -1, 2, 0.05, GM_RED);
    else if (paddles[0].position.y > 1.2)
      gm_draw_rectangle(0, 1, 2, 0.05, GM_RED);

  } while (gm_yield());
}

void gameover_scene() {
  do {
    gm_draw_text(0, 0, "Game Over", "", 0.5, GM_RED);
  } while (gm_yield());
}

int main() {
  gm_init(500, 500, "Gama test application");

  do {
    if (!gameover)
      pong_scene();
    else
      gameover_scene();
  } while (gm_yield());

  return 0;
}
