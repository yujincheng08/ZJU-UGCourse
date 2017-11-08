#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
vec3 Camera::direction() {
  return vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle),
              cos(verticalAngle) * cos(horizontalAngle));
}

vec3 Camera::right() {
  return vec3(-cos(horizontalAngle), 0.0, sin(horizontalAngle));
}

mat4 Camera::getProjectionMatrix(double ratio, double near, double far) {
  return perspective(glm::radians((float)fov), (float)ratio, (float)near,
                     (float)far);
}

mat4 Camera::getViewMatrix() {
  return lookAt(position, position + direction(), up());
}
