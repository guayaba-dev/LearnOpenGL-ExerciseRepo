#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

class camera {
  glm::vec3 target;
  glm::vec3 eye;

public:
  float yaw = 0.0f, pitch = 0.0f;

  void update(float _deltaTime = 0.0f) {}

  void moveCamera(glm::vec2 moveDir) {
    glm::vec3 right = glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat2x3 moveMat;
    moveMat[0] = target;
    moveMat[1] = right;

    eye += moveMat * moveDir;
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

    view = glm::lookAt(eye, eye + glm::vec3(target), glm::vec3(0, 1, 0));
  }
};
