#include "solar.h"
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
Solar::Solar() : Scene(1280, 720, "Solar") {
  sun = new Sphere(0xFFFF88, 1.0f, 50);
  earth = new Sphere(0x0088CC, 0.2f, 50);
  mars = new Sphere(0x888822, 0.2f, 50);
  moon = new Sphere(0xCCCCCC, 0.05f, 50);
  add(sun);
  add(earth);
  add(mars);
  add(moon);
}

void Solar::animate(double deltaTime) {
  static double earthAngle = M_PI;
  static double marsAngle = 0.0;
  static double moonAngle = M_PI_2;
  earthAngle += deltaTime * M_PI / 5;
  marsAngle += deltaTime * M_PI / 15;
  moonAngle += deltaTime * M_PI;
  vec3 earthPos(2.0f * sin(earthAngle), 2.0f * cos(earthAngle), 0.0f);
  vec3 moonPos =
      rotate(normalize(cross(earthPos, vec3(0.0f, 0.0f, 1.0f))) * 0.4f,
             (float)moonAngle, earthPos);
  earth->translate(earthPos);
  mars->translate(vec3(4.0f * sin(marsAngle), 0.0f, 4.0f * cos(marsAngle)));
  moon->translate(moonPos + earthPos);
}
