#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const char *vertexShaderSrc = "#version 330 core \n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "layout (location = 1) in vec3 aColor;\n"
                              "out vec3 outColor;\n"
                              "void main(){\n"
                              "gl_Position = vec4(aPos, 1.0);\n"
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

void processInputs(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void on_windows_size_callback(GLFWwindow *window, int WITDH, int HEIGHT) {
  glViewport(0, 0, WITDH, HEIGHT);
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "cap6", NULL, NULL);

  if (window == NULL) {
    std::cerr << "couldn't create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  glfwSetWindowSizeCallback(window, on_windows_size_callback);
  glViewport(0, 0, 800, 600);

  unsigned int VBO, VAO, vertexShader, fragShader, shaderProgram;

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

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << '\n';
  }

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << '\n';
  }

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
  }

  int vertexColorLoc = glGetUniformLocation(shaderProgram, "outColor");
  glUseProgram(shaderProgram);

  std::cerr << vertexColorLoc << "\n";

  while (!glfwWindowShouldClose(window)) {
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.f + 0.5;

    glUniform4f(vertexColorLoc, 0., greenValue, greenValue + 1, 1.);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    processInputs(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
