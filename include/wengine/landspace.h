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
#include "wengine/resource_manager.h"
#include "wengine/game_instance.h"

// namespace and class and function
using glm::mat4;
using glm::ortho;
using glm::radians;
using glm::rotate;
using glm::vec3;

namespace WEngine
{
  class ILandSpace
  {
  public:
    virtual void DrawLandSpace() = 0;
  };

  class LandSpace : public ILandSpace
  {
  public:
    float width = 800.0f, height = 600.0f;
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
    Shader shader;

    LandSpace(float width, float height);
    ~LandSpace();

    void Print();
    void Move(vec3 target);
    void MoveTo(mat4 target);
    void LookAt(mat4 target);
    virtual void DrawLandSpace() = 0;
    void setUp();
  };

  LandSpace::LandSpace(float width, float height)
      : width(width), height(height)
  {
    shader = ResourceManager::GetShader("landspace");
    if (!shader.IsValid())
    {
      shader = ResourceManager::LoadShader(FileSystem::getPath("landspace.vs").c_str(),
                                           FileSystem::getPath("landspace.fs").c_str(),
                                           nullptr, "landspace");
    }
  }

  LandSpace::~LandSpace()
  {
    if (NULL != indices)
      free(indices);
    indices = NULL;
    if (NULL != vertices)
      free(vertices);
    vertices = NULL;
  }

  void LandSpace::Move(vec3 target)
  {
    this->modelMatrix = glm::translate(this->modelMatrix, target);
  }

  void LandSpace::MoveTo(mat4 target)
  {
    this->modelMatrix = target;
  }

  void LandSpace::LookAt(mat4 target)
  {
    this->viewMatrix = target;
  }
  void LandSpace::setUp()
  {
    // 创建对象 buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定对象
    glBindVertexArray(VAO);
    // 绑定并加载数据到顶点buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vSize * sizeof(float), this->vertices, GL_STATIC_DRAW);
    // 绑定并加载数据到元素buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->iSize * sizeof(unsigned int), this->indices, GL_STATIC_DRAW);

    // 设置顶点属性点
    // 顶点位置数据
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  }

  void LandSpace::Print()
  {
    printf("vertices %u: \n", vSize / 3);
    for (uint32_t i = 0; i < vSize; i += 3)
    {
      printf("<%f,%f,%f>\n", vertices[i], vertices[i + 1], vertices[i + 2]);
    }
    printf("indices %u: \n", iSize / 3);
    for (uint32_t i = 0; i < iSize; i += 3)
    {
      printf("<%u,%u,%u>\n", indices[i], indices[i + 1], indices[i + 2]);
    }
  }

  class SphereLandSpace : public LandSpace
  {
  private:
    float radius;

    void GetVectors();

  public:
    SphereLandSpace(float radius);
    void DrawLandSpace();
  };

  SphereLandSpace::SphereLandSpace(float radius)
      : radius(radius), LandSpace(0, 0)
  {
    this->width = (1.0f / sqrt(3)) * radius;
    this->height = this->width;
    GetVectors();
    LandSpace::setUp();
  }

  void SphereLandSpace::DrawLandSpace()
  {
    shader.Use();

    this->viewMatrix = GameInstance::GetGlobalCamera().ViewMatrix();
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    shader.SetVector3f("aColor", this->color);
    shader.SetMatrix4("model", this->modelMatrix);
    shader.SetMatrix4("view", this->viewMatrix);
    shader.SetMatrix4("projection", this->projectionMatrix);

    // 渲染
    // 绑定VAO对象
    glBindVertexArray(VAO);
    // 绘制元素
    glBufferData(GL_ARRAY_BUFFER, this->vSize * sizeof(float), this->vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->iSize * sizeof(unsigned int), this->indices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);
    this->modelMatrix = rotate(this->modelMatrix, radians(-90.0f), vec3(0, 1.0f, 0));
    shader.SetMatrix4("model", this->modelMatrix);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);
    this->modelMatrix = rotate(this->modelMatrix, radians(-90.0f), vec3(0, 1.0f, 0));
    shader.SetMatrix4("model", this->modelMatrix);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);
    this->modelMatrix = rotate(this->modelMatrix, radians(-90.0f), vec3(0, 1.0f, 0));
    shader.SetMatrix4("model", this->modelMatrix);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);
    this->modelMatrix = rotate(this->modelMatrix, radians(-90.0f), vec3(1.0f, 0, 0));
    shader.SetMatrix4("model", this->modelMatrix);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);
    this->modelMatrix = rotate(this->modelMatrix, radians(-180.0f), vec3(1.0f, 0, 0));
    shader.SetMatrix4("model", this->modelMatrix);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);

    // 解绑VAO对象
    glBindVertexArray(0);
  }

  void SphereLandSpace::GetVectors()
  {
    float w = this->width;
    float h = this->height;
    float r = this->radius;
    unsigned int size = 9;
    // x
    std::vector<float> xCoords(size);
    // y
    std::vector<float> yCoords(size);
    xCoords[0] = -w / 2.0f;
    xCoords[size - 1] = w / 2.0f;
    yCoords[0] = -h / 2.0f;
    yCoords[size - 1] = h / 2.0f;

    float ws = 1.0f / (size - 1) * w;
    float hs = 1.0f / (size - 1) * h;
    for (unsigned int i = 1; i < (size - 1); i++)
    {
      xCoords[i] = -w / 2.0f + i * ws;
      yCoords[i] = -h / 2.0f + i * hs;
    }

    vSize = size * size * 3;
    float z = -(1.0f / sqrt(3)) * r * (1 / 2.0f);
    vertices = new float[vSize];
    unsigned int vv = 0;
    for (unsigned int i = 0; i < size; i++)
    {
      float yy = yCoords[i];
      for (unsigned int j = 0; j < size; j++)
      {
        float xx = xCoords[j];
        glm::vec3 tp = r * glm::normalize(glm::vec3(xx, yy, z));
        vertices[vv] = tp.x;
        vertices[vv + 1] = tp.y;
        vertices[vv + 2] = tp.z;
        vv += 3;
      }
    }

    iSize = (size - 1) * (size - 1) * 2 * 3;
    unsigned int ii = 0;
    indices = new unsigned int[iSize];
    for (unsigned int i = 0; i < (size - 1); i += 1)
    {
      for (unsigned int j = 0; j < (size - 1); j += 1)
      {
        indices[ii] = size * i + size + j;
        indices[ii + 1] = size * i + size + j + 1;
        indices[ii + 2] = size * i + j;
        indices[ii + 3] = size * i + size + j + 1;
        indices[ii + 4] = size * i + j + 1;
        indices[ii + 5] = size * i + j;
        ii += 6;
      }
    }
  }

  class PlaneLandSpace : public LandSpace
  {
  public:
    Texture2D texture;
    PlaneLandSpace(float width, float height);
    virtual void DrawLandSpace();
    void GetVectors();
  };

  PlaneLandSpace::PlaneLandSpace(float width, float height)
      : LandSpace(width, height)
  {
    shader = ResourceManager::LoadShader(FileSystem::getPath("landspace.vs").c_str(),
                                         FileSystem::getPath("landspace.fs").c_str(),
                                         nullptr, "landspace");
    texture = ResourceManager::LoadTexture(FileSystem::getPath("wall.jpg").c_str(), false, "wall");
    GetVectors();
    LandSpace::setUp();
  }

  void PlaneLandSpace::DrawLandSpace()
  {
    shader.Use();

    this->viewMatrix = GameInstance::GetGlobalCamera().ViewMatrix();
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    shader.SetVector3f("aColor", this->color);
    shader.SetMatrix4("model", this->modelMatrix);
    shader.SetMatrix4("view", this->viewMatrix);
    shader.SetMatrix4("projection", this->projectionMatrix);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    // 渲染
    // 绑定VAO对象
    glBindVertexArray(VAO);
    // 绘制元素
    glBufferData(GL_ARRAY_BUFFER, this->vSize * sizeof(float), this->vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->iSize * sizeof(unsigned int), this->indices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, this->iSize, GL_UNSIGNED_INT, 0);

    // 解绑VAO对象
    glBindVertexArray(0);
  }

  void PlaneLandSpace::GetVectors()
  {
    float w = this->width;
    float h = this->height;
    unsigned int size = 32;
    float lbx = -w / 2.0f;
    float lbz = h / 2.0f;
    float dx = w / (size - 1);
    float dz = h / (size - 1);

    vSize = size * size * 3;
    vertices = new float[vSize];
    int c = 0;
    for (int i = 0; i < vSize / 3; i++)
    {
      int idx = 3 * i;
      vertices[idx] = lbx + dx * (i % size);
      vertices[idx + 1] = 0;
      vertices[idx + 2] = lbz - dz * ((c / size) % size);
      c++;
    }

    iSize = (size - 1) * (size - 1) * 2 * 3;
    unsigned int ii = 0;
    indices = new unsigned int[iSize];
    for (unsigned int i = 0; i < (size - 1); i += 1)
    {
      for (unsigned int j = 0; j < (size - 1); j += 1)
      {
        indices[ii] = size * i + size + j;
        indices[ii + 1] = size * i + size + j + 1;
        indices[ii + 2] = size * i + j;
        indices[ii + 3] = size * i + size + j + 1;
        indices[ii + 4] = size * i + j + 1;
        indices[ii + 5] = size * i + j;
        ii += 6;
      }
    }
    // LandSpace::Print();
  }
}
