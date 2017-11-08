#include "solar.h"
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
Solar::Solar() : Scene(1280, 720, "Solar") {
  sun = new Sphere(0xFFFF88, 1.0f, 50);
  earth = new Sphere(0x0088CC, 0.2f, 50);
  mars = new Sphere(0x888822, 0.15f, 50);
  mercury = new Sphere(0xEEEEEE, 0.4f, 50);
  moon = new Sphere(0xAAAAAA, 0.05f, 50);
  satellite = new Sphere(0xCCCCCC, 0.1f, 50);
  add(sun);
  add(earth);
  add(mars);
  add(moon);
  add(satellite);
  add(mercury);
}

void Solar::animate(double deltaTime) {
  static double earthAngle = M_PI;
  static double marsAngle = 0.0;
  static double moonAngle = M_PI_2;
  static double mercuryAngle = 0.0;
  static double satelliteAngle = 0.0;
  earthAngle += deltaTime * M_PI / 5;
  marsAngle += deltaTime * M_PI / 15;
  moonAngle += deltaTime * M_PI;
  mercuryAngle += deltaTime * M_PI / 2;
  satelliteAngle += deltaTime * 2 * M_PI;

  vec3 earthPos(3.0f * sin(earthAngle), 3.0f * cos(earthAngle), 0.0f);
  vec3 moonPos(0.4f * sin(moonAngle), 0.4f * cos(moonAngle), 0.0f);
  vec3 mercuryPos(2.0f * sin(mercuryAngle), 2.0f * cos(mercuryAngle),
                  2.0f * sin(mercuryAngle));
  vec3 satellitePos(0.8f * sin(satelliteAngle), 0.0f,
                    0.8f * cos(satelliteAngle));
  earth->translate(earthPos);
  mars->translate(vec3(4.0f * sin(marsAngle), 0.0f, 4.0f * cos(marsAngle)));
  moon->translate(moonPos + earthPos);
  mercury->translate(mercuryPos);
  satellite->translate(satellitePos + mercuryPos);
}
