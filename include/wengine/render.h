#pragma once

// local
#include "wengine/shader.h"
#include "wengine/texture.h"

// opengl
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 命名空间和对象
using glm::vec2;
using glm::vec3;
namespace WEngine
{
  class Render
  {
  private:
    Shader shader;
    unsigned int quadVAO;
    void initRenderData();

  public:
    Render(Shader &shader);
    ~Render();
    void Draw(Texture2D &texture, vec2 position, vec2 size, float rotate, vec3 color);
  };

  Render::Render(Shader &shader)
  {
    this->shader = shader;
    this->initRenderData();
  }

  Render::~Render()
  {
    glDeleteVertexArrays(1, &this->quadVAO);
  }

  void Render::Draw(Texture2D &texture, vec2 position, vec2 size, float rotate, vec3 color)
  {
    // 应用着色器
    this->shader.Use();
    // 矩阵变换
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    //设置uniform参数
    // 模型矩阵
    this->shader.SetMatrix4("model", model);
    // 颜色三元组
    this->shader.SetVector3f("planetColor", color);

    // 纹理通道绑定
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    //渲染
    //绑定VAO
    glBindVertexArray(this->quadVAO);
    //绘制
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //解除绑定
    glBindVertexArray(0);
  }

  void Render::initRenderData()
  {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}
