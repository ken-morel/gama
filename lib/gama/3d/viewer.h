#ifndef GAMA_3D_VIEWER_H_INCLUDED
#define GAMA_3D_VIEWER_H_INCLUDED
#include "transform.h"
#include <math.h>

/* ================================================================
   QUATERNION HELPERS (to handle view-aligned rotations without gimbal lock or
   unwanted roll)
   ================================================================ */

typedef struct {
  double w, x, y, z;
} _gmu_quat;

_gmu_quat quat_identity(void) {
  _gmu_quat q = {1.0, 0.0, 0.0, 0.0};
  return q;
}

_gmu_quat quat_mult(_gmu_quat a, _gmu_quat b) {
  _gmu_quat c;
  c.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
  c.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
  c.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
  c.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
  return c;
}

_gmu_quat quat_from_axis_angle(double ax, double ay, double az,
                               double angle_deg) {
  double angle_rad = angle_deg * M_PI / 180.0;
  double half_angle = angle_rad / 2.0;
  double s = sin(half_angle);
  double c = cos(half_angle);
  _gmu_quat q = {c, ax * s, ay * s, az * s};
  double norm = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
  if (norm > 1e-10) {
    q.w /= norm;
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;
  }
  return q;
}

void quat_to_matrix(_gmu_quat q, double m[3][3]) {
  double xx = q.x * q.x;
  double xy = q.x * q.y;
  double xz = q.x * q.z;
  double xw = q.x * q.w;
  double yy = q.y * q.y;
  double yz = q.y * q.z;
  double yw = q.y * q.w;
  double zz = q.z * q.z;
  double zw = q.z * q.w;
  m[0][0] = 1.0 - 2.0 * (yy + zz);
  m[0][1] = 2.0 * (xy - zw);
  m[0][2] = 2.0 * (xz + yw);
  m[1][0] = 2.0 * (xy + zw);
  m[1][1] = 1.0 - 2.0 * (xx + zz);
  m[1][2] = 2.0 * (yz - xw);
  m[2][0] = 2.0 * (xz - yw);
  m[2][1] = 2.0 * (yz + xw);
  m[2][2] = 1.0 - 2.0 * (xx + yy);
}

void quat_to_euler(_gmu_quat q, double *pitch, double *yaw, double *roll) {
  double m[3][3];
  quat_to_matrix(q, m);
  *pitch = -asin(m[1][2]) * 180.0 / M_PI;
  double cp = cos(*pitch * M_PI / 180.0);
  if (fabs(cp) < 1e-5) {
    // Gimbal lock, set yaw to 0, roll to something (but we clamp to avoid this)
    *yaw = 0.0;
    *roll = atan2(m[1][0], m[1][1]) * 180.0 / M_PI;
  } else {
    *yaw = atan2(m[0][2], m[2][2]) * 180.0 / M_PI;
    *roll = atan2(m[1][0], m[1][1]) * 180.0 / M_PI;
  }
}

_gmu_quat euler_to_quat(double pitch, double yaw, double roll) {
  double p = pitch * M_PI / 180.0 / 2.0;
  double y = yaw * M_PI / 180.0 / 2.0;
  double r = roll * M_PI / 180.0 / 2.0;
  double sp = sin(p);
  double cp = cos(p);
  double sy = sin(y);
  double cy = cos(y);
  double sr = sin(r);
  double cr = cos(r);
  _gmu_quat q;
  q.w = cy * cp * cr + sy * sp * sr;
  q.x = cy * sp * cr - sy * cp * sr;
  q.y = sy * cp * cr + cy * sp * sr;
  q.z = sy * sp * cr - cy * cp * sr;
  return q;
}

/* ================================================================
   GLOBAL CONFIGURATION (edit these defaults once at program startup)
   ================================================================ */

typedef struct {
  double rotation_sensitivity; // degrees per mouse pixel (orbit)
  double pan_sensitivity;      // world units per mouse pixel (Shift+drag)
  double zoom_speed;      // world units per mouse pixel per second (Ctrl+drag)
  double key_move_speed;  // world units per second (arrow keys)
  double key_scale_speed; // scale multiplier per second (+ / - keys)
  double min_scale;       // prevent objects from becoming too tiny
  double max_pitch;       // clamp vertical rotation to avoid gimbal lock / flip
} gm3ModelViewConfig;

/* Change any value you like right after you initialise your app */
gm3ModelViewConfig gm3_model_view_config = {
    .rotation_sensitivity = 0.18, // nice responsive orbit
    .pan_sensitivity = 0.012,     // smooth panning
    .zoom_speed = 0.75,           // comfortable zoom with Ctrl+drag
    .key_move_speed = 4.0,        // arrow keys → 4 units/second
    .key_scale_speed =
        0.5, // + / - → 50% scale change per second (multiplicative)
    .min_scale = 0.05,
    .max_pitch = 89.0};

/* ================================================================
   MODEL VIEWER BINDINGS (only two multipliers)
   ================================================================ */
void gm3_model_view_bindings(
    gm3Transform *t,
    double mouse_mult, // Scales all mouse sensitivities (rot, pan, zoom)
    double key_mult)   // Scales all key speeds (move, scale)
{
  /* Mouse orbit (plain left drag) - view-aligned via quaternion to avoid
   * sideways roll */
  if (gm_mouse.down && !gm_key('S') && !gm_key('C')) {
    _gmu_quat current =
        euler_to_quat(t->rotation.x, t->rotation.y, t->rotation.z);

    double dx = gm_mouse.movement.x;
    double dy = gm_mouse.movement.y;
    double sens = gm3_model_view_config.rotation_sensitivity * mouse_mult;

    // Incremental rotations around global axes
    _gmu_quat delta_yaw = quat_from_axis_angle(0.0, 1.0, 0.0, dx * sens);
    _gmu_quat delta_pitch = quat_from_axis_angle(1.0, 0.0, 0.0, dy * sens);

    // Apply in global space (post-multiply)
    current = quat_mult(current, delta_yaw);
    current = quat_mult(current, delta_pitch);

    // Extract euler, discard any induced roll for view alignment
    double p, y, r;
    quat_to_euler(current, &p, &y, &r);
    t->rotation.x = p;
    t->rotation.y = y;
    t->rotation.z = 0.0; // Force no roll to prevent sideways twisting

    // Clamp pitch
    if (t->rotation.x > gm3_model_view_config.max_pitch)
      t->rotation.x = gm3_model_view_config.max_pitch;
    if (t->rotation.x < -gm3_model_view_config.max_pitch)
      t->rotation.x = -gm3_model_view_config.max_pitch;
  }

  /* Mouse pan (Shift + left drag) */
  if (gm_mouse.down && gm_key('S') && !gm_key('C')) {
    t->position.x -= gm_mouse.movement.x *
                     gm3_model_view_config.pan_sensitivity * mouse_mult;
    t->position.y += gm_mouse.movement.y *
                     gm3_model_view_config.pan_sensitivity *
                     mouse_mult; // +Y = up
  }

  /* Mouse zoom (Ctrl + left drag vertical) */
  if (gm_mouse.down && gm_key('C') && !gm_key('S')) {
    t->position.z += gm_mouse.movement.y * gm3_model_view_config.zoom_speed *
                     mouse_mult * gm_dt();
  }

  /* Arrow keys – translation (world units per second) */
  if (gm_key('U'))
    t->position.y += gm3_model_view_config.key_move_speed * key_mult * gm_dt();
  if (gm_key('D'))
    t->position.y -= gm3_model_view_config.key_move_speed * key_mult * gm_dt();
  if (gm_key('L'))
    t->position.x -= gm3_model_view_config.key_move_speed * key_mult * gm_dt();
  if (gm_key('R'))
    t->position.x += gm3_model_view_config.key_move_speed * key_mult * gm_dt();

  /* + and - keys – uniform scaling (multiplicative for better feel) */
  if (gm_key('+')) {
    double factor =
        1.0 + gm3_model_view_config.key_scale_speed * key_mult * gm_dt();
    t->scale.x *= factor;
    t->scale.y *= factor;
    t->scale.z *= factor;
  }
  if (gm_key('-')) {
    double factor =
        1.0 - gm3_model_view_config.key_scale_speed * key_mult * gm_dt();
    if (factor < 0.0)
      factor = 0.0;
    t->scale.x *= factor;
    t->scale.y *= factor;
    t->scale.z *= factor;

    /* Enforce minimum scale */
    if (t->scale.x < gm3_model_view_config.min_scale)
      t->scale.x = gm3_model_view_config.min_scale;
    if (t->scale.y < gm3_model_view_config.min_scale)
      t->scale.y = gm3_model_view_config.min_scale;
    if (t->scale.z < gm3_model_view_config.min_scale)
      t->scale.z = gm3_model_view_config.min_scale;
  }

  /* Space = full reset */
  if (gm_key(' ')) {
    *t = gm3_transform();
  }
}

#endif // GAMA_3D_VIEWER_H_INCLUDED
