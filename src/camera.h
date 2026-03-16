#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include <iostream>

struct Camera {
  glm::vec3 target = glm::vec3(0.0f);
  glm::vec3 eye = glm::vec3(0.0f);
  float yaw = 0.0f, pitch = 0.0f;

public:
  Camera() = default;

  void moveCamera(glm::vec2 moveDir) {
    glm::vec3 right = glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat2x3 moveMat;

    eye += target * moveDir.x;
    eye += right * moveDir.y;
  }

  void panningCamera(int radius, float cameraOffset, glm::mat4 &view) {

    target = glm::vec3(0.0f);

    eye = glm::vec3(radius * glm::cos(cameraOffset), 0.0f,
                    radius * glm::sin(cameraOffset));

    view = glm::lookAt(eye, target, glm::vec3(0, 1, 0));
  }

  void movingCamera(glm::mat4 &view) {
    target.x = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    target.y = glm::sin(glm::radians(pitch));
    target.z = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    view = glm::lookAt(eye, eye + glm::normalize(target), glm::vec3(0, 1, 0));
  }
};
