#include "../../include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"
#include "../shader.h"

#define STB_CUBESVERTICES_IMPLEMENTATION
#include "../cubesVertices.h"

float lastXPos = 400.0f, lastYPos = 300.0f;

float yaw = 0.0f, pitch = 0.0f;

glm::vec3 eye;
glm::vec3 cameraFront;

float moveSpeed = 0.05f;

bool isCameraPanning = false;

void processInput(GLFWwindow *window) {

  glm::vec3 right = glm::cross(cameraFront, glm::vec3(0, 1, 0));

  right = glm::normalize(right);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    eye += cameraFront * moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    eye -= cameraFront * moveSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    eye += right * moveSpeed;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    eye -= right * moveSpeed;
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos) {
  float xOffset = x_pos - lastXPos;
  float yOffset = y_pos - lastYPos;

  lastXPos = x_pos;
  lastYPos = y_pos;

  float sentivity = 0.1;

  xOffset *= sentivity;
  yOffset *= sentivity;

  yaw -= xOffset;
  pitch -= yOffset;

  pitch = (pitch >= 89.0f) ? 89.0f : pitch;
  pitch = (pitch <= -89.0f) ? -89.0f : pitch;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
    isCameraPanning = !isCameraPanning;
    yaw = 0.0f, pitch = 0.0f;
  }
}

void panningCamera(glm::vec3 &eye, int radius, glm::mat4 &view) {

  cameraFront = glm::vec3(0.0f);

  eye = glm::vec3(radius * glm::cos(glfwGetTime()), 0.0f,
                  radius * glm::sin(glfwGetTime()) - 0.0f);

  view = glm::lookAt(eye, cameraFront, glm::vec3(0, 1, 0));
}

void movingCamera(glm::vec3 &eye, glm::mat4 &view) {

  cameraFront.x = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  cameraFront.y = glm::sin(glm::radians(pitch));
  cameraFront.z = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

  view = glm::lookAt(eye, eye + glm::vec3(cameraFront), glm::vec3(0, 1, 0));
}

void glfwSizeCallBack(GLFWwindow *window, int WIDTH, int HEIGHT) {
  glViewport(0, 0, WIDTH, HEIGHT);
}

void loadTexture(unsigned char *textureData, int width, int height,
                 int format) {

  if (textureData) {
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "error loading image";
  }
}

int main(int argc, char *argv[]) {

  glfwInit();

  GLFWwindow *window;

  window = glfwCreateWindow(800, 600, "Cap10", NULL, NULL);

  if (!window)
    return -1;

  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetKeyCallback(window, key_callback);

  glfwSetCursorPosCallback(window, mouse_callback);

  ////////////////////////////
  ///
  /// PREPARING SCENE
  ///
  ///////////////////////////

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return -1;

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, 800, 600);

  glfwSetWindowSizeCallback(window, glfwSizeCallBack);

  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        (void *)(sizeof(float) * 3));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  int width, height, nrchannels;
  unsigned char *imageData = stbi_load("assets/textures/container.jpg", &width,
                                       &height, &nrchannels, 0);

  if (imageData) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "error loading image";
  }

  STBI_FREE(imageData);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  imageData = stbi_load("assets/textures/awesomeface.png", &width, &height,
                        &nrchannels, 0);

  if (imageData) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "error loading image";
  }

  STBI_FREE(imageData);

  Shader shader =
      Shader("assets/shaders/cap9/cap9.vert", "assets/shaders/cap9/cap9.frag");

  shader.use();

  ///////////////
  ///
  /// GLM
  ///
  //////////////

  int radius = 5.f;

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);

  glm::mat4 proj =
      glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE,
                     glm::value_ptr(proj));

  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  glClearColor(0, 0, 0, 1);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    (isCameraPanning) ? panningCamera(eye, radius, view)
                      : movingCamera(eye, view);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));

    for (auto i : cubePositions) {

      glm::mat4 view = glm::mat4(1.0f);
      model = glm::translate(view, i);

      glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                         glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
