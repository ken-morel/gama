typedef struct {
  double focal;
  double near;
  double far;
} gm3Camera;

const gm3Camera gm3_default_camera = {
    .far = 100,
    .near = 0.01,
    .focal = 1.3,
};

int gm3_camera_create(gm3Camera *c) {
  if (!c)
    return -1;
  *c = gm3_default_camera;
  return 0;
}
