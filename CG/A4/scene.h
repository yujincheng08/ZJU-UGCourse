#ifndef SCENE_H
#define SCENE_H
#include "camera.h"
#include <string>
#include <vector>

namespace GL {
#include <GL/glew.h>
}
class GLFWwindow;
class Mesh;

class Scene {
  bool initialized = false;
  GLFWwindow *window = nullptr;
  Camera camera;
  GL::GLuint programID = 0;
  int width, height;
  std::string title;
  GL::GLuint textureSampleID;
  GL::GLuint projectionMatrixID;
  GL::GLuint viewMatrixID;
  GL::GLuint translationMatrixID;
  GL::GLuint rotationMatrixID;
  GL::GLuint scaleMatrixID;
  std::vector<Mesh *> meshSet;
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  void setupCallback();

  void loadShaders(std::string vertex, std::string fragment);
  void moveCamera(double deltaTime);

protected:
  virtual void keyCallback(int key, int scancode, int action, int mods);
  virtual void mouseButtonCallback(int button, int action, int mods);
  virtual void cursorPosCallback(double xpos, double ypos);
  virtual void scrollCallback(double xoffset, double yoffset);
  virtual void framebufferSizeCallback(int width, int height);

  virtual void animate(double deltaTime);

public:
  Scene(unsigned width = 1280u, unsigned height = 720u, std::string title = "");
  Scene(std::string title) : Scene(1280u, 720u, title) {}
  void setCamera(Camera &camera) { this->camera = camera; }
  void setBackground(double r, double g, double b, double a);
  Camera &getCamera() { return camera; }
  int exec();

  void add(Mesh *mesh);
};

#endif // SCENE_H
