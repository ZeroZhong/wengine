#pragma once

// local
#include "wengine/shader.h"
#include "wengine/texture.h"

// vender
#include "stb/stb_image.h"

// opengl
#include <glad/glad.h>

// std
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// 命名空间和使用对象
using std::exception;
using std::ifstream;
using std::map;
using std::string;
using std::stringstream;

namespace WEngine
{
  class FileSystem
  {
  public:
    static string getPath(const string &path)
    {
      string basePath = "E:/work/std/c/d/resource/";
      return basePath + path;
    }
  };

  class ResourceManager
  {
  private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char *file, bool alpha);

  public:
    static map<string, Shader> Shaders;
    static map<string, Texture2D> Textures;
    static Shader &LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, string name);
    static Shader &GetShader(string name);
    static Texture2D &LoadTexture(const char *file, bool alpha, string name);
    static Texture2D &GetTexture(string name);
    static void Clear();
  };

  map<string, Texture2D> ResourceManager::Textures;
  map<string, Shader> ResourceManager::Shaders;

  Shader &ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile,
                                      const char *gShaderFile, string name)
  {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
  }

  Shader &ResourceManager::GetShader(string name)
  {
    return Shaders[name];
  }

  Texture2D &ResourceManager::LoadTexture(const char *file, bool alpha, string name)
  {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
  }

  Texture2D &ResourceManager::GetTexture(string name)
  {
    return Textures[name];
  }

  void ResourceManager::Clear()
  {
    for (auto iter : Shaders)
    {
      glDeleteProgram(iter.second.ID);
    }

    for (auto iter : Textures)
    {
      glDeleteTextures(1, &iter.second.ID);
    }
  }

  Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile,
                                             const char *gShaderFile)
  {
    // 1. 从文件路径读取 vertex/fragment代码
    string vertexCode;
    string fragmentCode;
    string geometryCode;
    try
    {
      // 打开文件
      ifstream vertexShaderFile(vShaderFile);
      ifstream fragmentShaderFile(fShaderFile);
      stringstream vShaderStream, fShaderStream;
      // 读取到字符流中
      vShaderStream << vertexShaderFile.rdbuf();
      fShaderStream << fragmentShaderFile.rdbuf();
      // 关闭文件
      vertexShaderFile.close();
      fragmentShaderFile.close();
      // 从字符流中获取着色程序代码
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      // 如何几何着色程序路径存在,加载几何着色程序
      if (gShaderFile != nullptr)
      {
        ifstream geometryShaderFile(gShaderFile);
        stringstream gShaderStream;
        gShaderStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        geometryCode = gShaderStream.str();
      }
    }
    catch (exception e)
    {
      std::cout << "读取着色程序失败" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 创建着色器
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
  }

  Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
  {
    // 创建纹理对象
    Texture2D texture;
    if (alpha)
    {
      texture.Internal_Format = GL_RGBA;
      texture.Image_Format = GL_RGBA;
    }
    // 加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
    // 生成纹理
    texture.Generate(width, height, data);
    // 释放图片数据
    stbi_image_free(data);
    return texture;
  }
}
