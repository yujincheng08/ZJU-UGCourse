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
  float x1, y1, x2, y2, z1, z2;
  float delta1, delta2, delta3, delta4, radius1, radius2;
  for (unsigned w = 0; w < resolution; w++) {
    for (int h = -(resolution / 2); h < (int)(resolution / 2); h++) {
      delta1 = (w / (float)resolution) * 2 * M_PI;
      delta2 = ((w + 1) / (float)resolution) * 2 * M_PI;

      delta3 = (h / (float)resolution) * M_PI;
      delta4 = ((h + 1) / (float)resolution) * M_PI;

      x1 = sin(delta1);
      y1 = cos(delta1);
      x2 = sin(delta2);
      y2 = cos(delta2);

      radius1 = radius * cos(delta3);
      radius2 = radius * cos(delta4);

      z1 = radius * sin(delta3);
      z2 = radius * sin(delta4);

      vec3 v;
      v = vec3(radius1 * x1, z1, radius1 * y1);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(radius1 * x2, z1, radius1 * y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(radius2 * x2, z2, radius2 * y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(radius1 * x1, z1, radius1 * y1);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(radius2 * x2, z2, radius2 * y2);
      vertices.push_back(v);
      texture.push_back(vec2(0.0f, 0.0f));
      normals.push_back(v);

      v = vec3(radius2 * x1, z2, radius2 * y1);
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
