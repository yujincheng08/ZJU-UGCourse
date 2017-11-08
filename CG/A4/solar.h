#ifndef SOLAR_H
#define SOLAR_H

#include "scene.h"
#include "sphere.h"
class Solar : public Scene {
  Sphere *sun;
  Sphere *earth;
  Sphere *mars;
  Sphere *moon;
  Sphere *mercury;
  Sphere *satellite;
  void animate(double deltaTime) override;

 public:
  Solar();
};

#endif  // SOLAR_H
