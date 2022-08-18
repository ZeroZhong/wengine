#pragma once

#include <string>
#include <vector>

// opengl
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// local
#include "wengine/shader.h"

// namespace and class
using namespace std;

#define MAX_BONE_INFLUENCE 4

namespace WEngine
{
  struct Vertex
  {
    // 位置
    glm::vec3 Position;
    // 法线
    glm::vec3 Normal;
    // 纹理
    glm::vec2 TexCoords;
    // 切线
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
  };

  struct Texture
  {
    unsigned int id;
    string type;
    string path;
  };

  class Mesh
  {
  private:
    // 渲染数据
    unsigned int VBO, EBO;

  public:
    // 网格数据
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // VAO
    unsigned int VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader &shader);
    void setupMesh();
  };

  Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
  {
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
  }

  Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
  {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
  }

  void Mesh::Draw(Shader &shader)
  {
    // 绑定适当的纹理
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
      // 在绑定之前,激活适当的纹理槽位
      glActiveTexture(GL_TEXTURE0 + i);
      // 获取纹理槽位号 (N in diffuse_textureN)
      string number;
      string name = textures[i].type;
      if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "texture_specular")
        number = std::to_string(specularNr++);
      else if (name == "texture_normal")
        number = std::to_string(normalNr++);
      else if (name == "texture_height")
        number = std::to_string(heightNr++);

      // 现在，将采样器设置为正确的纹理单位
      glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
      // 最终绑定纹理
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // 渲染
    // 绑定VAO对象
    glBindVertexArray(VAO);
    // 绘制元素
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    // 解绑VAO对象
    glBindVertexArray(0);

    // 处理完成，将所有内容设置回默认值,这是好的做法。
    glActiveTexture(GL_TEXTURE0);
  }

  void Mesh::setupMesh()
  {
    // 创建对象 buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定对象
    glBindVertexArray(VAO);
    // 绑定并加载数据到顶点buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    // 绑定并加载数据到元素buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 设置顶点属性点
    // 顶点位置数据
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // 顶点法线数据
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // 纹理
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // 顶点切线
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // vector bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));
    // IDs
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
  }
}