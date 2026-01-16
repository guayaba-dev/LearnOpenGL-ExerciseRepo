#include "../../include/glad/glad.h"

#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>

float triangle[]{
    -1.0f, -0.5f, 0.0f, // Bottom left
    -0.5f, 0.5f,  0.0f, // Top left
    0.0f,  -0.5f, 0.0f, // Bottom shared
    0.5f,  0.5f,  0.0f, // Top Right
    1.0f,  -0.5f, 0.0f  // bottom Right
};

unsigned int indices[]{
    0, 1, 2, // first triangle
    2, 3, 4, // second trinagle
};

const char *vertexShaderSrc = // Vertex Shader
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "gl_Position = vec4(aPos, 1.f);}\0";

const char *fragShaderSrcA = // Frag Shader
    "#version  330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(1.f, 1.f, 1.f, 1.f);}\0";

const char *fragShaderSrcB = // Frag Shader
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(1.f, 1.f, 0.f, 1.f);}\0";

void proccessInput(GLFWwindow *window) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void frame_buffer_size_callback(GLFWwindow *window, int WIDTH, int HEIGHT) {
  glViewport(0, 0, WIDTH, HEIGHT);
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, 3);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Exer 3", NULL, NULL);

  if (window == NULL) {
    std::cerr << "GLFW could't create a window \n";
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "FAILED TO INITIALIZE GLAD\n";
    return -1;
  }

  unsigned int VAO, VBO, EAO, vertexShader, fragShaderA, fragShaderB,
      progShaderA, progShaderB;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];

  fragShaderA = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShaderA, 1, &fragShaderSrcA, NULL);
  glCompileShader(fragShaderA);

  fragShaderB = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShaderB, 1, &fragShaderSrcB, NULL);
  glCompileShader(fragShaderB);

  progShaderA = glCreateProgram();
  glAttachShader(progShaderA, vertexShader);
  glAttachShader(progShaderA, fragShaderA);
  glLinkProgram(progShaderA);

  progShaderB = glCreateProgram();
  glAttachShader(progShaderB, vertexShader);
  glAttachShader(progShaderB, fragShaderB);
  glLinkProgram(progShaderB);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

  glClearColor(0.f, 0.f, 0.f, 1.f);

  while (!glfwWindowShouldClose(window)) {
    proccessInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(progShaderA);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glUseProgram(progShaderB);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   (void *)(sizeof(unsigned int) * 3));

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
