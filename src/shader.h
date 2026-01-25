#ifndef __SHADER_H__
#define __SHADER_H__

#include "../include/glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {

public:
  unsigned int ID;

  Shader(const char *vertexPath,
         const char *framentPath); // Defined outside the class

  void use() // use shader
  {
    glUseProgram(ID);
  }
  // utility uniform functions
  void setBool(const std::string name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  };
  void setDouble(const std::string name, double value) const {

    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setInt(const std::string name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
};

Shader::Shader(const char *vertexPath, const char *framentPath) {

  std::string vShaderSrc;
  std::string fShaderSrc;
  std::ifstream vFile;
  std::ifstream fFile;

  vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vFile.open(vertexPath);
    fFile.open(framentPath);
    std::stringstream vertexSteam, fragmentSteam;

    vertexSteam << vFile.rdbuf();
    fragmentSteam << fFile.rdbuf();

    vFile.close();
    fFile.close();

    vShaderSrc = vertexSteam.str();
    fShaderSrc = fragmentSteam.str();

  } catch (std::fstream::failure) {

    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY\n";
  }

  const char *vShaderCode = vShaderSrc.c_str();
  const char *fShaderCode = fShaderSrc.c_str();

  // compiling the shaders

  unsigned int fragmentShader, vertexShader;
  int success;
  char infoLog[512];

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {

    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << "\n";
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {

    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << "\n";
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << "\n";
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

#endif
