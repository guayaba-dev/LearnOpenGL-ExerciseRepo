#include "../../include/glad/glad.h"

#include "../../include/glm/gtc/type_ptr.hpp"
#include "../camera.h"
#include "../shader.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_CUBESVERTICES_IMPLEMENTATION
#include "../cubesVertices.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

Camera camera = Camera();

int lastXPos = 0.0f, lastYPos = 0.0f;

float moveSpeed = 0.05f;

bool isCameraPanning = false;

void processInput(GLFWwindow *window) {

  glm::vec2 moveVec = glm::vec2(0.0f);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    moveVec.x = 1.0f;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    moveVec.x = -1.0f;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    moveVec.y = 1.0f;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    moveVec.y = -1.0f;

  if (glm::abs(moveVec.x) + glm::abs(moveVec.y) != 0)
    moveVec = glm::normalize(moveVec);

  camera.moveCamera(moveVec * moveSpeed);
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos) {

  float xOffset = x_pos - lastXPos;
  float yOffset = y_pos - lastYPos;

  lastXPos = x_pos;
  lastYPos = y_pos;

  float sentivity = 0.1;

  xOffset *= sentivity;
  yOffset *= sentivity;

  camera.yaw -= xOffset;
  camera.pitch -= yOffset;

  camera.pitch = (camera.pitch >= 89.0f) ? 89.0f : camera.pitch;
  camera.pitch = (camera.pitch <= -89.0f) ? -89.0f : camera.pitch;

  std::cerr << camera.yaw << "\n";
  std::cerr << camera.pitch << "\n";
  printf("x: %f y: %f z: %f\n", camera.target.x, camera.target.y,
         camera.target.z);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
    isCameraPanning = !isCameraPanning;

    camera.yaw = glm::degrees(glm::atan(-camera.target.z, -camera.target.x));
    camera.pitch = 0.0f;
  }
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

  unsigned int VAO, VBO, lightVAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPosOnly), verticesPosOnly,
               GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  glGenVertexArrays(1, &lightVAO);

  glBindVertexArray(lightVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  Shader objectShader = Shader("assets/shaders/cap11/shader.vert",
                               "assets/shaders/cap11/objectShader.frag");

  Shader lightShader = Shader("assets/shaders/cap11/shader.vert",
                              "assets/shaders/cap11/lightShader.frag");

  ///////////////
  ///
  /// GLM
  ///
  //////////////

  int radius = 5.f;

  glm::mat4 lightModel = glm::mat4(1.0f);
  glm::mat4 objectModel = glm::mat4(1.0f);

  lightModel = glm::translate(lightModel, glm::vec3(-1.0));
  objectModel = glm::translate(objectModel, glm::vec3(2.0));

  glm::mat4 view = glm::mat4(1.0f);

  glm::mat4 proj =
      glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  glClearColor(0, 0, 0, 1);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    (isCameraPanning) ? camera.panningCamera(radius, (float)glfwGetTime(), view)
                      : camera.movingCamera(view);

    lightShader.use();

    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1,
                       GL_FALSE, glm::value_ptr(proj));

    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1,
                       GL_FALSE, glm::value_ptr(lightModel));

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    objectShader.use();

    glUniform3f(glGetUniformLocation(objectShader.ID, "ambientColor"), 1.0f,
                0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(objectShader.ID, "lightColor"), 1.0f, 1.0f,
                1.0f);

    glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "projection"), 1,
                       GL_FALSE, glm::value_ptr(proj));

    glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(glGetUniformLocation(objectShader.ID, "model"), 1,
                       GL_FALSE, glm::value_ptr(objectModel));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
