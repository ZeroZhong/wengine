#pragma once

// std
#include <iostream>

// opengl
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// namespace
using glm::mat4;
using glm::value_ptr;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::string;

namespace WEngine
{
  class Shader
  {
  public:
    unsigned int ID;
    Shader() {}
    // 编译和使用
    Shader &Use();
    void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);

    // uniform变量 工具函数
    void SetBool(const string &name, bool value, bool useShader = false);
    void SetInt(const string &name, int value, bool useShader = false);
    void SetFloat(const string &name, float value, bool useShader = false);
    void SetVector2f(const char *name, float x, float y, bool useShader = false);
    void SetVector2f(const char *name, const vec2 &value, bool useShader = false);
    void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);
    void SetVector3f(const char *name, const vec3 &value, bool useShader = false);
    void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f(const char *name, const vec4 &value, bool useShader = false);
    void SetMatrix4(const string &name, const mat4 &matrix, bool useShader = false);

    bool IsValid();

  private:
    bool valid = false;
    void checkCompileErrors(unsigned int object, string type);
  };

  bool Shader::IsValid()
  {
    return this->valid;
  }

  Shader &Shader::Use()
  {
    glUseProgram(this->ID);
    return *this;
  }

  void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
  {
    unsigned int sVertex, sFragment, gShader;
    // 顶点 Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // 片段 Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    if (geometrySource != nullptr)
    {
      // 几何 Shader
      gShader = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(gShader, 1, &geometrySource, NULL);
      glCompileShader(gShader);
      checkCompileErrors(gShader, "GEOMETRY");
    }
    // 创建shader程序
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
      glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // 删除所有的Shader.当他们关联到shader程序之后,shader的代码就不在需要了
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
      glDeleteShader(gShader);

    this->valid = true;
  }

  void Shader::SetBool(const string &name, bool value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
  }

  void Shader::SetInt(const string &name, int value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
  }

  void Shader::SetFloat(const string &name, float value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
  }

  void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
  }

  void Shader::SetVector2f(const char *name, const vec2 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
  }

  void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
  }
  void Shader::SetVector3f(const char *name, const vec3 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
  }

  void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
  }

  void Shader::SetVector4f(const char *name, const vec4 &value, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
  }

  void Shader::SetMatrix4(const string &name, const mat4 &matrix, bool useShader)
  {
    if (useShader)
      this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, value_ptr(matrix));
  }

  void Shader::checkCompileErrors(unsigned int object, string type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(object, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(object, 1024, NULL, infoLog);
        std::cout << "编译Shader程序错误 类型: " << type << "," << infoLog << std::endl;
      }
    }
    else
    {
      glGetProgramiv(object, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(object, 1024, NULL, infoLog);
        std::cout << "编译Shader程序错误 类型: " << type << "," << infoLog << std::endl;
      }
    }
  }
}