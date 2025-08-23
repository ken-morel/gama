#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "../app.h"

int init(App *);
int create();
int update(float theta);
int render();
int shutdown();

App *GamaCreateApp();

int main() {
  App *app = GamaCreateApp();
  GLFWwindow *window;

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW!\n");
    return -1;
  }

  // Set window hints (for modern OpenGL contexts, optional)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  init(app);

  /* Create a windowed mode window and its OpenGL context */

  window = glfwCreateWindow(app->size.x, app->size.y, app->title, NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window!\n");
    glfwTerminate();
    return -1;
  }

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
