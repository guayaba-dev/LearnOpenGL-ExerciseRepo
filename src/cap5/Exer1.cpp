#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int witdh, int height) {
  glViewport(0, 0, witdh, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  GLFWwindow *window = glfwCreateWindow(800, 600, "Exer-1", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create a GLFW window \n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
      -1.0f, -0.5f, 0.0f, // Bottom left
      -0.5f, 0.5f,  0.0f, // Top left
      0.0f,  -0.5f, 0.0f, // Bottom shared
      0.5f,  0.5f,  0.0f, // Top Right
      1.0f,  -0.5f, 0.0f, // bottom Right
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 2, // first Triangle
      2, 3, 4  // second Triangle
  };

  const char *vertexShaderSrc = "#version 330 core \n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main(){\n"
                                "gl_Position = vec4(aPos, 1.0);}\0";

  const char *fragmentShaderSrc = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "void main(){\n"
                                  "FragColor = vec4(1.0, 1.0, 1.0, 1.0);}\0";

  unsigned int VAO;            // VERTEX ARRAY OBJECT
  unsigned int EBO;            // ELEMENT ARRAY OBJECT
  unsigned int VBO;            // VERTEX BUFFER OBJECT
  unsigned int vertexShader;   // VERTEX SHADER
  unsigned int fragmentShader; // FRAGMENT SHADER
  unsigned int shaderProgram;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << '\n';
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << '\n';
  }

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
  }

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // RENDERING LOGIC
    glClearColor(.0, .0, .0, 1.);
    glClear(GL_COLOR_BUFFER_BIT);

    glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glfwTerminate();
  return 0;
}
