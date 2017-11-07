#ifndef SPHERE_H
#define SPHERE_H
#include <cstdint>
#include "mesh.h"
#include "scene.h"

class Sphere : public Mesh {
 public:
  Sphere(uint32_t color, float radius = 1, unsigned resolution = 10u);
};

#endif  // SPHERE_H
