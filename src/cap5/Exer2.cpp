#include "../../include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

float triagleA[]{
    -1.0f, -0.5f, 0.0f, // Bottom left
    -0.5f, 0.5f,  0.0f, // Top left
    0.0f,  -0.5f, 0.0f  // Bottom shared
};

float triangleB[]{
    0.0f, -0.5f, 0.0f, // Bottom shared
    0.5f, 0.5f,  0.0f, // Top Right
    1.0f, -0.5f, 0.0f  // bottom Right
};

const char *vertexSrc =    // vertex code
    "#version 330 core \n" // GLSL version
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "gl_Position = vec4(aPos, 1.f);}\0";

const char *fragSrc = // frag code
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(1.f, 1.f,1.f,1.f);}\0";

void framebuffer_size_callback(GLFWwindow *window, int witdh, int height) {
  glViewport(0, 0, witdh, height);
}

void proccessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Exer-2", NULL, NULL);

  if (window == NULL) {
    std::cerr << "GLFW couldn't create a window\n";
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

  // SHADER DATA
  unsigned int vertexShader;
  unsigned int fragShader;
  unsigned int progShader;

  char infoLog[512];
  int success;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSrc, NULL);
  glCompileShader(vertexShader);

  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragSrc, NULL);
  glCompileShader(fragShader);

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

  progShader = glCreateProgram();
  glAttachShader(progShader, vertexShader);
  glAttachShader(progShader, fragShader);
  glLinkProgram(progShader);

  if (!success) {
    glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::::COMPILATION_FAILED\n" << infoLog << '\n';
  }
  // SEND DATA TO OPEN GL
  unsigned int VAO_A;
  unsigned int VBO_A;

  unsigned int VAO_B;
  unsigned int VBO_B;

  glGenVertexArrays(1, &VAO_A);
  glGenBuffers(1, &VBO_A);

  glBindVertexArray(VAO_A);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_A);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triagleA), triagleA, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  glGenVertexArrays(1, &VAO_B);
  glGenBuffers(1, &VBO_B);

  glBindVertexArray(VAO_B);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_B);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleB), triangleB, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glUseProgram(progShader);

  while (!glfwWindowShouldClose(window)) {
    proccessInput(window);

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO_A);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_A);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(VAO_B);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_B);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);

  glfwTerminate();
  return 0;
}
