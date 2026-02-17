#include "../../include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <stdatomic.h>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#include "../../include/stb_image.h"
#include "../shader.h"

float vertices[] = {
    // positions                     // colors                 // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
};

unsigned int indices[] = {0, 1, 3,  // first Triangle
                          1, 2, 3}; // second Triangle

void processInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void glfw_windows_size_change_callback(GLFWwindow *window, int HEIGHT,
                                       int WIDTH) {
  glViewport(0, 0, HEIGHT, WIDTH);
}

int main() {

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Cap-8", NULL, NULL);

  if (window == NULL)
    return -1;

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return -1;

  glViewport(0, 0, 800, 600);

  glfwSetWindowSizeCallback(window, glfw_windows_size_change_callback);

  unsigned int VAO, VBO, EAO, Texture1, Texture2;

  glGenVertexArrays(1, &VAO);

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(sizeof(float) * 3));

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                        (void *)(sizeof(float) * 6));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glGenTextures(1, &Texture1);

  glBindTexture(GL_TEXTURE_2D, Texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data = stbi_load("assets/textures/container.jpg", &width,
                                  &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture A\n";
  }
  stbi_image_free(data);

  Shader shader =
      Shader("assets/shaders/cap9.vert", "assets/shaders/cap9.frag");
  shader.use();

  glClearColor(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, Texture1);
    shader.setInt("texture1", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
