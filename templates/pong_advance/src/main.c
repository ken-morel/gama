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
  ball_body.restitution = 1.3;
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
  double paddley = 0;

  do {
    double dt = gm_dt();
    printf("%lf\n", dt);
    gm_max_speed_anim(&ball_body, 0.6, gm_anim_linear, dt, 0.3);
    gm_max_speed_anim(&goal_body, 0.1, gm_anim_linear, dt, 0.3);
    if (fabs(goal_body.position.x) > 1)
      goal_body.position.x *= -0.99;
    if (fabs(goal_body.position.y) > 1)
      goal_body.position.y *= -0.95;

    if (gm_system_get_collision(&sys, &ball_body, &goal_body) != NULL) {
      score++;
    } else if (fabs(ball_body.position.x) >= 1) {
      score--;
      if (ball_body.position.x > 1)
        ball_body.position.x = -1;
      else if (ball_body.position.x < -1)
        ball_body.position.x = 1;
    } else if (score < 0) {
      score = 10;
      // return;
    }

    if (fabs(ball_body.velocity.x) >= 0.5) {
      ball_body.velocity.x -= 0.01;
    }

    if (gm_key('U'))
      paddley += 50 * dt;
    else if (gm_key('D'))
      paddley -= 50 * dt;
    else
      paddley = 0;
    gm_anim_linear(&paddles[0].velocity.y, paddley, dt, 0.1);
    gm_anim_linear(&paddles[1].velocity.y, paddley, dt, 0.1);

    sprintf(score_text, "Score: %d", score);

    gm_system_update(&sys);

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
