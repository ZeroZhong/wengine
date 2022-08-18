#pragma once

// opengl
#include <glad/glad.h>

namespace WEngine
{
  class Texture2D
  {
  private:
  public:
    unsigned int ID;
    unsigned int Width, Height;
    // 纹理格式
    unsigned int Internal_Format; // 内部纹理对象的格式
    unsigned int Image_Format;    // 图片格式
    // 纹理配置
    unsigned int Wrap_S;     //覆盖模式 U轴
    unsigned int Wrap_T;     //覆盖模式 V轴
    unsigned int Filter_Min; // 过滤模式 if 纹理 pixels < 屏幕 pixels
    unsigned int Filter_Max; // 过滤模式 if 纹理 pixels > 屏幕 pixels
    Texture2D();
    void Generate(unsigned int width, unsigned int height, unsigned char *data);
    void Bind() const;
  };

  Texture2D::Texture2D()
      : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB),
        Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
  {
    glGenTextures(1, &this->ID);
  }

  void Texture2D ::Generate(unsigned int width, unsigned int height, unsigned char *data)
  {
    this->Width = width;
    this->Height = height;
    // 创建纹理
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    // 设置纹理的覆盖和过滤模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Texture2D ::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, this->ID);
  }

}
