#ifndef CAMERA_H
#define CAMERA_H
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

class Camera {
  glm::vec3 position = glm::vec3(10, 0, 0);
  double horizontalAngle = -M_PI_2;
  double verticalAngle = 0.0;
  double fov = 45.0;

  glm::vec3 direction();
  glm::vec3 right();
  glm::vec3 up(glm::vec3 &right, glm::vec3 direction()) {
    return glm::cross(right, direction());
  }
  glm::vec3 up() { return glm::cross(right(), direction()); }

 public:
  Camera() {}
  void forward(double step) { position += direction() * float(step); }
  void backward(double step) { position -= direction() * float(step); }
  void left(double step) { position -= right() * float(step); }
  void right(double step) { position += right() * float(step); }
  void turnHorizontally(double step) { horizontalAngle += step; }
  void turnVertically(double step) { verticalAngle += step; }
  void up(double step) { position += up() * float(step); }
  void down(double step) { position -= up() * float(step); }
  void zoom(double step) { fov -= step; }
  void tilt(double /*deltaTime*/) {}
  glm::mat4 getProjectionMatrix(double ratio, double near = 0.1,
                                double far = 1000.0);
  glm::mat4 getViewMatrix();
};

#endif  // CAMERA_H
