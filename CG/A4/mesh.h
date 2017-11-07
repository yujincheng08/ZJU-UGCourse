#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <vector>
#include "scene.h"
namespace GL {
#include <GL/glew.h>
}

class Mesh {
  GL::GLuint vertexBuffer = 0u;
  GL::GLuint textureBuffer = 0u;
  GL::GLuint normalBuffer = 0u;
  GL::GLuint elementBuffer = 0u;

  GL::GLuint texture = 0u;
  GL::GLuint vertexID = 0u;
  GL::GLuint uvID = 0u;

  GL::GLuint textureSampleID = 0u;

  GL::GLuint translationMatrixID = 0u;
  GL::GLuint rotationMatrixID = 0u;
  GL::GLuint scaleMatrixID = 0u;

  glm::mat4 translationMatrix = glm::mat4(1.0f);
  glm::mat4 rotationMatrix = glm::mat4(1.0f);
  glm::mat4 scaleMatrix = glm::mat4(1.0f);

  size_t size = 0;

 public:
  Mesh();

  void draw();

  void setTranslationMatrixID(GL::GLuint id) { translationMatrixID = id; }
  void setRotationMatrixID(GL::GLuint id) { rotationMatrixID = id; }
  void setScaleMatrixID(GL::GLuint id) { scaleMatrixID = id; }

  void setTextureSampleID(GL::GLuint id) { textureSampleID = id; }

  void setVertexID(GL::GLuint id) { vertexID = id; }
  void setUVID(GL::GLuint id) { uvID = id; }

  void translate(glm::vec3 vector);
  void rotate(glm::vec3 axis, float angle);
  void scale(glm::vec3 vector);

 protected:
  void enableBuffers(std::vector<glm::vec3> vertices,
                     std::vector<glm::vec2> uvs,
                     std::vector<glm::vec3> normals);
  void enableTexture(std::vector<unsigned char> data, unsigned width,
                     unsigned height, unsigned type);
};

#endif  // MESH_H
