#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "../app.h"
#include <math.h>

void init(App *app);
void create();
void update(float);
void render();
void gama_click(App *, MouseClickEvent *);
App *GamaCreateApp();

void setGLPerspective(float width, float height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLdouble aspect_ratio = (GLdouble)width / (GLdouble)height;
  GLdouble near_plane = -1.0;
  GLdouble far_plane = 1.0;
  GLdouble fov_y_radians =
      60.0 * (113.0 / 355.0) / 180.0; // 60 degrees in radians
  GLdouble top = tan(fov_y_radians / 2.0) * near_plane;
  GLdouble bottom = -top;
  GLdouble right = top * aspect_ratio;
  GLdouble left = -right;
  glFrustum(left, right, bottom, top, near_plane, far_plane);

  glMatrixMode(GL_MODELVIEW);
}

App *gama;
void __handleClick(GLFWwindow *win, int button, int action, int mode) {
  double posx, posy;
  int width, height;
  glfwGetCursorPos(win, &posx, &posy);
  glfwGetWindowSize(win, &width, &height);
  float hw = (float)width / 2.0f, hh = (float)height / 2.0f;
  MouseClickEvent *event = (MouseClickEvent *)malloc(sizeof(MouseClickEvent));
  event->x = (posx - hw) / hw;
  event->y = (posy - hh) / hh * -1;
  gama_click(gama, event);
}

int main() {
  gama = GamaCreateApp();
  GLFWwindow *window;

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW!\n");
    return -1;
  }

  // Set window hints (for modern OpenGL contexts, optional)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  init(gama);

  /* Create a windowed mode window and its OpenGL context */
  if (gama->width != gama->height) {
    printf("Sorry, but non-square windows are not yet supported");
    exit(500);
  }

  window = glfwCreateWindow(gama->width, gama->height, gama->title, NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window!\n");
    glfwTerminate();
    return -1;
  }
  glfwSetMouseButtonCallback(window, __handleClick);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  create();

  double lastFrameTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastFrameTime);
    lastFrameTime = currentTime;

    // Process events (polling for events)
    glfwPollEvents();
    update(deltaTime);
    glClear(GL_COLOR_BUFFER_BIT);
    render();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
