#include "sphere.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;
using namespace glm;
Sphere::Sphere(uint32_t color, float radius, unsigned resolution) {
  vector<vec3> vertices;
  vector<vec2> texture;
  vector<vec3> normals;

  // iniatiate the variable we are going to use
  float X1, Y1, X2, Y2, Z1, Z2;
  float inc1, inc2, inc3, inc4, Radius1, Radius2;
  // unsigned count = 0u;
  for (unsigned w = 0; w < resolution; w++) {
    for (int h = -(resolution / 2); h < (int)(resolution / 2); h++) {
      inc1 = (w / (float)resolution) * 2 * M_PI;
      inc2 = ((w + 1) / (float)resolution) * 2 * M_PI;

      inc3 = (h / (float)resolution) * M_PI;
      inc4 = ((h + 1) / (float)resolution) * M_PI;

      X1 = sin(inc1);
      Y1 = cos(inc1);
      X2 = sin(inc2);
      Y2 = cos(inc2);

      // store the upper and lower radius, remember everything is going to be
      // drawn as triangles
      Radius1 = radius * cos(inc3);
      Radius2 = radius * cos(inc4);

      Z1 = radius * sin(inc3);
      Z2 = radius * sin(inc4);

      vec3 v;
      v = vec3(Radius1 * X1, Z1, Radius1 * Y1);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(Radius1 * X2, Z1, Radius1 * Y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(Radius2 * X2, Z2, Radius2 * Y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(Radius1 * X1, Z1, Radius1 * Y1);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(Radius2 * X2, Z2, Radius2 * Y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(Radius2 * X1, Z2, Radius2 * Y1);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);
    }
  }
  enableBuffers(vertices, texture, normals);

  const vector<unsigned char> data = {
      (unsigned char)((color & uint32_t(0xFF0000)) >> 16),
      (unsigned char)((color & uint32_t(0x00FF00)) >> 8),
      (unsigned char)(color & uint32_t(0x0000FF)),
  };
  enableTexture(data, 1, 1, GL_RGB);
}
