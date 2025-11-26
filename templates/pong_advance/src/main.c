#include "animate.h"
#include <gama.h>
#include <stdio.h>

int gameover = 0;

void pong_scene() {
  int score = 1;

  char score_text[20];

  gmSystem sys = gm_system_create();

  gmBody ball_body = gm_circle_body(100, 0, 0, 0.2);
  ball_body.velocity.y = 0.4;
  ball_body.velocity.x = 0.4;
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

  gmBody goal_body = gm_circle_body(500, 0, -0.5, 0.05);
  gm_system_push(&sys, &goal_body);

  do {
    double dt = gm_dt();
    printf("%lf", dt);
    gm_max_speed_anim(&ball_body, 0.5, gm_anim_spring, dt, 1);
    gm_max_speed_anim(&goal_body, 0.1, gm_anim_spring, dt, 1);

    if (gm_collision_detect(&ball_body, &goal_body)) {
      score++;
    } else if (fabs(ball_body.position.x) >= 1) {
      score--;
      if (ball_body.position.x > 1)
        ball_body.position.x = -1;
      else if (ball_body.position.x < -1)
        ball_body.position.x = 1;
    } else if (score < 0) {
      // gameover = 1;
      score = 10;
      // return;
    }

    if (fabs(ball_body.velocity.x) >= 0.5) {
      ball_body.velocity.x -= 0.01;
    }

    double velocity = gm_key('U') ? -4 : gm_key('D') ? 4 : 0;
    gm_anim_spring(&paddles[0].velocity.y, velocity, dt, 0.1);
    gm_anim_spring(&paddles[1].velocity.y, velocity, dt, 0.1);

    sprintf(score_text, "Score: %d", score);

    gm_system_update(&sys, 500);

    gm_draw_circle_body(&ball_body, GM_BISQUE);
    gm_draw_rect_bodies(paddles, 2, GM_DARKGOLDENROD);
    gm_draw_circle_body(&goal_body, GM_GREEN);
    gm_draw_text(0, 0.8, score_text, 0.2, GM_BLACK);
  } while (gm_yield());
}

void gameover_scene() {}

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
