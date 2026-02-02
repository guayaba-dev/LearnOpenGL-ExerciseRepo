#include "../../include/glad/glad.h"
#include "../../include/stb_image.h"

#include "../shader.h"

#include <GLFW/glfw3.h>
#include <iostream>

float mixAmount = 0;

void processInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    mixAmount += 0.01;

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    mixAmount -= 0.01;

  mixAmount = (mixAmount > 1) ? 1. : mixAmount;
  mixAmount = (mixAmount < 0) ? 0. : mixAmount;
}

void glfw_windows_size_change_callback(GLFWwindow *window, int HEIGHT,
                                       int WIDTH) {

  glViewport(0, 0, HEIGHT, WIDTH);
}

float vertices[] = {
    // positions                     // colors                 // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
};

unsigned int indices[] = {0, 1, 3,  // first Triangle
                          1, 2, 3}; // second Triangle

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

  unsigned int VAO, VBO, EAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  unsigned int boxTexture, smileTexture;
  glGenTextures(1, &boxTexture);
  glGenTextures(1, &smileTexture);

  glBindTexture(GL_TEXTURE_2D, boxTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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

  glBindTexture(GL_TEXTURE_2D, smileTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("assets/textures/awesomeface.png", &width, &height,
                   &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture B\n";
  }
  stbi_image_free(data);

  Shader shader = Shader("./assets/shaders/cap7/shader.vert",
                         "./assets/shaders/cap7/Exer4.frag");
  shader.use();

  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  glClearColor(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);
    shader.setDouble("mixAmount", mixAmount);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, boxTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, smileTexture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
