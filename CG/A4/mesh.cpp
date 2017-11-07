#include "mesh.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
Mesh::Mesh() {  // initialBuffers();
}

using namespace GL;
void Mesh::draw() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(textureSampleID, 0);
  glUniformMatrix4fv(translationMatrixID, 1, GL_FALSE,
                     glm::value_ptr(translationMatrix));
  glUniformMatrix4fv(rotationMatrixID, 1, GL_FALSE,
                     glm::value_ptr(rotationMatrix));
  glUniformMatrix4fv(scaleMatrixID, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
  glEnableVertexAttribArray(vertexID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(uvID);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

  glDisableVertexAttribArray(vertexID);
  glDisableVertexAttribArray(uvID);
}

void Mesh::translate(glm::vec3 vector) {
  translationMatrix = glm::translate(glm::mat4(1.0f), vector);
}

void Mesh::rotate(glm::vec3 vector, float angle) {
  rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, vector);
}
void Mesh::scale(glm::vec3 vector) {
  scaleMatrix = glm::scale(glm::mat4(1.0f), vector);
}

struct PackedVertex {
  glm::vec3 vertex;
  glm::vec2 uv;
  glm::vec3 normal;
  bool operator<(const PackedVertex &that) const {
    return memcmp((void *)this, (void *)&that, sizeof(PackedVertex)) > 0;
  }
};

void Mesh::enableBuffers(std::vector<glm::vec3> vertices,
                         std::vector<glm::vec2> uvs,
                         std::vector<glm::vec3> normals) {
  std::map<PackedVertex, unsigned short> vertexIndex;
  std::vector<glm::vec3> indexedVertices;
  std::vector<glm::vec2> indexedUVs;
  std::vector<glm::vec3> indexedNormals;
  std::vector<GLushort> indces;

  if (vertices.size() != uvs.size() || uvs.size() != normals.size()) return;
  for (size_t i = 0; i != vertices.size(); ++i) {
    PackedVertex pack = {vertices[i], uvs[i], normals[i]};
    auto iter = vertexIndex.find(pack);
    if (iter == vertexIndex.end()) {
      indexedVertices.push_back(pack.vertex);
      indexedUVs.push_back(pack.uv);
      indexedNormals.push_back(pack.normal);
      GLushort index = (GLushort)indexedVertices.size() - 1;
      indces.push_back(index);
      vertexIndex[pack] = index;
    } else {
      indces.push_back(iter->second);
    }
  }

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(glm::vec3),
               &indexedVertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &textureBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glBufferData(GL_ARRAY_BUFFER, indexedUVs.size() * sizeof(glm::vec2),
               &indexedUVs[0], GL_STATIC_DRAW);

  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3),
               &indexedNormals[0], GL_STATIC_DRAW);

  glGenBuffers(1, &elementBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, elementBuffer);
  glBufferData(GL_ARRAY_BUFFER, indces.size() * sizeof(GLushort), &indces[0],
               GL_STATIC_DRAW);

  size = indces.size();
}

void Mesh::enableTexture(std::vector<unsigned char> data, unsigned width,
                         unsigned height, unsigned type) {
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type,
               GL_UNSIGNED_BYTE, &data[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
