#include "scene.h"
#include "sphere.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace glm;
using namespace std;
using namespace GL;

static unordered_map<GLFWwindow *, Scene *> callBackMap;

#include <string.h>
void Scene::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  auto sceneIterator = callBackMap.find(window);
  if (sceneIterator != callBackMap.end())
    sceneIterator->second->keyCallback(key, scancode, action, mods);
}

void Scene::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                int mods) {
  auto sceneIterator = callBackMap.find(window);
  if (sceneIterator != callBackMap.end())
    sceneIterator->second->mouseButtonCallback(button, action, mods);
}

void Scene::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  auto sceneIterator = callBackMap.find(window);
  if (sceneIterator != callBackMap.end())
    sceneIterator->second->cursorPosCallback(xpos, ypos);
}

void Scene::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  auto sceneIterator = callBackMap.find(window);
  if (sceneIterator != callBackMap.end())
    sceneIterator->second->scrollCallback(xoffset, yoffset);
}

void Scene::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  auto sceneIterator = callBackMap.find(window);
  if (sceneIterator != callBackMap.end()) {
    if (width == 0)
      width = 1;
    if (height == 0)
      height = 1;
    sceneIterator->second->framebufferSizeCallback(width, height);
  }
}

void Scene::setupCallback() {
  glfwSetKeyCallback(window, keyCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, cursorPosCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Scene::keyCallback(int key, int, int action, int mods) {
  /*   double step = 0.5;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      switch (key) {
        case GLFW_KEY_W:
          camera.forward(step);
          break;
        case GLFW_KEY_S:
          camera.backward(step);
          break;
        case GLFW_KEY_A:
          camera.left(step);
          break;
        case GLFW_KEY_D:
          camera.right(step);
          break;
        case GLFW_KEY_SPACE:
          if (mods & GLFW_MOD_SHIFT)
            camera.down(step);
          else
            camera.up(step);
          break;
        default:
          break;
      }
    } */
}

void Scene::mouseButtonCallback(int, int action, int) {
  static double x, y;
  static int mode;
  if (action == GLFW_PRESS) {
    glfwGetCursorPos(window, &x, &y);
    mode = glfwGetInputMode(window, GLFW_CURSOR);
    glfwSetCursorPos(window, width / 2, height / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else if (action == GLFW_RELEASE) {
    glfwSetCursorPos(window, x, y);
    glfwSetInputMode(window, GLFW_CURSOR, mode);
  }
}

void Scene::cursorPosCallback(double xpos, double ypos) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glfwSetCursorPos(window, width / 2, height / 2);
    camera.turnHorizontally((width / 2 - xpos) * 0.005);
    camera.turnVertically((height / 2 - ypos) * 0.005);
  }
}

void Scene::scrollCallback(double, double yoffset) { camera.zoom(yoffset); }

void Scene::framebufferSizeCallback(int width, int height) {
  this->width = width, this->height = height;
  glViewport(0, 0, width, height);
}

void Scene::animate(double deltaTime) {}

void Scene::moveCamera(double deltaTime) {
  int step = 5;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.forward(step * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.backward(step * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.left(step * deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.right(step * deltaTime);
  }
}

Scene::Scene(unsigned w, unsigned h, string t) : width(w), height(h), title(t) {
  if (!glfwInit()) {
    cerr << "Failed to initialzie GLFW" << endl;
    return;
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to open GLFW window." << endl;
    glfwTerminate();
    return;
  }
  callBackMap.insert(make_pair(window, this));
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    return;
  }
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  loadShaders("vertexShader", "fragmentShader");
  textureSampleID = glGetUniformLocation(programID, "texture");
  projectionMatrixID = glGetUniformLocation(programID, "ProjectionMatrix");
  viewMatrixID = glGetUniformLocation(programID, "ViewMatrix");
  translationMatrixID = glGetUniformLocation(programID, "TranslationMatrix");
  rotationMatrixID = glGetUniformLocation(programID, "RotationMatrix");
  scaleMatrixID = glGetUniformLocation(programID, "ScaleMatrix");
  initialized = true;
}

void Scene::setBackground(double r, double g, double b, double a) {
  glClearColor(r, g, b, a);
}

int Scene::exec() {
  if (!initialized)
    return EXIT_FAILURE;

  setupCallback();
  double oldTime = glfwGetTime();
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    double newTime = glfwGetTime();
    animate(newTime - oldTime);
    moveCamera(newTime - oldTime);
    mat4 Projction = camera.getProjectionMatrix((float)width / height);
    mat4 View = camera.getViewMatrix();

    glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE,
                       glm::value_ptr(Projction));
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(View));

    for (auto &mesh : meshSet) {
      mesh->draw();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
    oldTime = newTime;
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
  glDeleteProgram(programID);
  return EXIT_SUCCESS;
}

void Scene::loadShaders(string vertex, string fragment) {
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  ifstream in(vertex, ios::in);
  string VertexShaderCode, FragmentShaderCode;
  if (in.is_open()) {
    stringstream sstream;
    sstream << in.rdbuf();
    VertexShaderCode = sstream.str();
    in.close();
  } else {
    cerr << "Impossible to open " << vertex << "!" << endl;
    return;
  }

  in.open(fragment, ios::in);
  if (in.is_open()) {
    stringstream sstream;
    sstream << in.rdbuf();
    FragmentShaderCode = sstream.str();
    in.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  const char *code = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &code, nullptr);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0 && Result == GL_FALSE) {
    char *msg = new char[InfoLogLength + 1];
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, msg);
    cerr << msg << endl;
    delete msg;
  }

  code = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &code, nullptr);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0 && Result == GL_FALSE) {
    char *msg = new char[InfoLogLength + 1];
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, msg);
    cerr << msg << endl;
    delete msg;
  }

  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0 && Result == GL_FALSE) {
    char *msg = new char[InfoLogLength + 1];
    glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, msg);
    cerr << msg << endl;
    delete msg;
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  programID = ProgramID;
}

void Scene::add(Mesh *mesh) {
  meshSet.push_back(mesh);
  mesh->setRotationMatrixID(rotationMatrixID);
  mesh->setScaleMatrixID(scaleMatrixID);
  mesh->setTextureSampleID(textureSampleID);
  mesh->setTranslationMatrixID(translationMatrixID);
  mesh->setVertexID(0);
  mesh->setUVID(1);
}
