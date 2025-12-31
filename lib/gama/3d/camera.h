typedef struct {
  double focal;
  double near;
  double far;
} gm3Camera;

int gm3_camera_create(gm3Camera *c) {
  if (!c)
    return -1;

  c->near = 0.1;
  c->far = 100;
  c->focal = 1.3;
  return 0;
}
