#include "../../include/glad/glad.h"

#include <GLFW/glfw3.h>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#include "../../include/stb_image.h"
#include "../shader.h"

#include "../cubesVertices.hpp"
#define STB_CUBESVERTICES_IMPLEMENTATION

void processInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void glfwSizeCallBack(GLFWwindow *window, int WIDTH, int HEIGHT) {

  glViewport(0, 0, WIDTH, HEIGHT);
}

int main(int argc, char *argv[]) {

  glfwInit();

  GLFWwindow *window;

  window = glfwCreateWindow(800, 600, "Cap10", NULL, NULL);

  if (!window)
    return -1;

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return -1;

  glViewport(0, 0, 800, 600);

  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VAO);

  glClearColor(0, 0, 0, 1);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
