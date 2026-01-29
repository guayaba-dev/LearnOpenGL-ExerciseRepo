#include "../../include/glad/glad.h"
#include "../../include/stb_image.h"

#include "../shader.h"

#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void glfw_windows_size_change_callback(GLFWwindow *window, int HEIGHT,
                                       int WIDTH) {

  glViewport(0, 0, HEIGHT, WIDTH);
}

float vertices[] = {
    // POSITIONS                           //COLORS

    -0.5f, -0.5f, 0.0f, // Bottom left
    0.5f,  -0.5f, 0.0f, // bottom Right
    0.f,   0.5f,  0.0f  // Top
};

int main() {

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Cap7", NULL, NULL);

  if (window == NULL)
    return -1;

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));

  glViewport(0, 0, 800, 600);

  glfwSetWindowSizeCallback(window, glfw_windows_size_change_callback);

  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  int width, height, nrChannels;
  unsigned char *data = stbi_load("assets/textures/container.jpg", &width,
                                  &height, &nrChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  Shader shader =
      Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");

  shader.use();

  glClearColor(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  stbi_image_free(data);

  return 0;
}
