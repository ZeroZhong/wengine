#pragma once

// std
#include <vector>

// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// local
#include "wengine/shader.h"

namespace WEngine
{
  class ICube
  {
  public:
    virtual void DrawCube(Shader &shader) = 0;
  };
  class Cube : public ICube
  {
  public:
    float width, height;
    float *vertices;
    unsigned int vSize = 0;

    unsigned int *indices;
    unsigned int iSize = 0;
    mat4 modelMatrix = mat4(1.0f);
    mat4 viewMatrix = mat4(1.0f);
    mat4 projectionMatrix = mat4(1.0f);
    vec3 color = vec3(1.0f, 1.0f, 1.0f);

    unsigned int VAO;
    unsigned int VBO, EBO;
    Cube();
    ~Cube();

    void GetVectors();
    void setUp();
    void DrawCube(Shader &shader);
  };

  Cube::Cube(float width, float height)
      : width(width), height(height)
  {
  }

  Cube::~Cube()
  {
    if (NULL != indices)
      free(indices);
    indices = NULL;
    if (NULL != vertices)
      free(vertices);
    vertices = NULL;
  }

  void Cube::GetVectors()
  {
  }
  void Cube::setUp()
  {
  }

  void DrawCube(Shader &shader)
  {
  }
}
