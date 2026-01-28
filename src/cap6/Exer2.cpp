#include "../../include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSrc =
    "#version 330 core \n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "uniform float vertexOffset;\n"

    "out vec3 outColor;\n"
    "void main(){\n"
    "vec4 offset = vec4(vertexOffset, 0, 0, 0);"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) + offset;\n"
    "outColor = aColor;\n"
    "}\0";
const char *fragmentShaderSrc = "#version 330 core                 \n"
                                "out vec4 FragColor;               \n"
                                "in vec3 outColor;                 \n"
                                "void main() {                     \n"
                                "  FragColor = vec4(outColor, 1.0);\n"
                                "}\0";

float vertices[] = {
    // POSITIONS                           //COLORS

    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom left
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom Right
    0.f,   0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // Top
};

float offset = 0;

void glfwProcessInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    offset += 0.2;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    offset -= 0.2;
}

void glfw_window_size_changed_callback(GLFWwindow *window, int WIDTH,
                                       int HEIGHT) {
  glViewport(0, 0, WIDTH, HEIGHT);
}

int main() {

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Cap6-Exer1", NULL, NULL);

  if (window == NULL) {
    std::cerr << "ERROR::GLFW::WINDOW::CREATE\n";
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "ERROR::GLAD::LOAD\n";
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetWindowSizeCallback(window, glfw_window_size_changed_callback);

  unsigned int VAO, VBO, vShader, fShader, shaderProgram;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vShader);

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vShader);
  glAttachShader(shaderProgram, fShader);
  glLinkProgram(shaderProgram);

  GLint uniformLoc = glGetUniformLocation(shaderProgram, "vertexOffset");

  while (!glfwWindowShouldClose(window)) {
    glUseProgram(shaderProgram);
    glUniform1f(uniformLoc, offset);
    glfwProcessInput(window);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
