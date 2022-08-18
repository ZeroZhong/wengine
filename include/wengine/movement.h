#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::cos;
using glm::cross;
using glm::mat4;
using glm::normalize;
using glm::radians;
using glm::rotate;
using glm::scale;
using glm::sin;
using glm::translate;
using glm::vec3;
using glm::vec4;

namespace WEngine
{
  class IMovement
  {
  public:
    virtual void SetPosition(vec3 position) = 0;
    virtual void SetScale(vec3 scale) = 0;
    virtual void SetAngles(float pitch, float yaw, float roll) = 0;
    virtual void Move(vec3 target) = 0;
    virtual void MoveTo(vec3 target) = 0;
  };

  class Movement : public IMovement
  {
  public:
    // 坐标变换：位置,缩放 ，默认生成的对象在世界原点，以这个作为锚点
    vec3 position = vec3(0, 0, 0);
    vec3 scale = vec3(1.0f, 1.0f, 1.0f);

    // 坐标变换：基本变换矩阵
    mat4 normalMatrix = mat4(1.0f);

    // 姿态：右手坐标系 : [前,右,上]
    vec3 direction = vec3(0, 0, 1.0f);
    vec3 right = vec3(0, 1.0f, 0);
    vec3 up = vec3(1.0f, 0, 0);

    // 姿态：三个角度
    float pitch = 0, yaw = 0, roll = 0;

    glm::vec3 speed = vec3(0, 0, 0);

    Movement();
    void SetPosition(vec3 position);
    void SetScale(vec3 scale);
    void SetAngles(float pitch, float yaw, float roll);

    void Move(vec3 target);
    void MoveTo(vec3 target);
    void PrintMatrix();
  };

  Movement::Movement()
  {
  }
  void Movement::PrintMatrix()
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        std::cout << normalMatrix[i][j] << " ";
      }
      std::cout << "\n";
    }
  }
  void Movement::SetPosition(vec3 position)
  {
    this->position = position;
    //平移
    this->normalMatrix[0][3] = this->position.x;
    this->normalMatrix[1][3] = this->position.y;
    this->normalMatrix[2][3] = this->position.z;
  }

  void Movement::SetScale(vec3 scale)
  {
    this->scale = scale;
    //缩放,(保留原来的旋转)
    this->normalMatrix[0][0] = this->scale.x * cos(this->yaw) * cos(this->roll);
    this->normalMatrix[1][1] = this->scale.y * cos(this->pitch) * cos(this->roll);
    this->normalMatrix[2][2] = this->scale.z * cos(this->pitch) * cos(this->yaw);
  }

  void Movement::SetAngles(float deltaPitch, float deltaYaw, float deltaRoll)
  {

    this->pitch = glm::radians(this->pitch + deltaPitch);
    this->yaw = glm::radians(this->yaw + deltaYaw);
    this->roll = glm::radians(this->roll + deltaRoll);
    // printf("dp,dy,dr : %f %f %f\n", this->pitch, this->yaw, this->roll);
    // 翻滚角不影响 向前方向姿态
    this->direction.x = cos(this->pitch) * sin(this->yaw);
    this->direction.y = sin(this->pitch);
    this->direction.z = cos(this->pitch) * cos(this->yaw);
    // 俯仰角不影响 向右方向姿态
    this->right.x = cos(this->roll) * cos(this->yaw);
    this->right.y = sin(this->roll);
    this->right.z = cos(this->roll) * sin(this->yaw);
    // 偏航角不影响 向上方向姿态
    this->up.x = sin(this->roll) * cos(this->pitch);
    this->up.y = cos(this->roll);
    this->up.z = sin(this->roll) * sin(this->pitch);

    //旋转,(保留原来的缩放)
    this->normalMatrix[0][0] = this->scale.x * cos(this->yaw) * cos(this->roll);
    this->normalMatrix[1][1] = this->scale.y * cos(this->pitch) * cos(this->roll);
    this->normalMatrix[2][2] = this->scale.z * cos(this->pitch) * cos(this->yaw);
    //沿x轴旋转,顺时针为正
    // this->normalMatrix[1][1] = cos(this->pitch);
    this->normalMatrix[1][2] = -sin(this->pitch);
    this->normalMatrix[2][1] = sin(this->pitch);
    // this->normalMatrix[2][2] = cos(this->pitch);
    //沿y轴旋转
    // this->normalMatrix[0][0] = cos(this->yaw);
    this->normalMatrix[0][2] = sin(this->yaw);
    this->normalMatrix[2][0] = -sin(this->yaw);
    // this->normalMatrix[2][2] = cos(this->yaw);
    //沿z轴旋转
    // this->normalMatrix[0][0] = cos(this->roll);
    this->normalMatrix[0][1] = -sin(this->roll);
    this->normalMatrix[1][0] = sin(this->roll);
    // this->normalMatrix[1][1] = cos(this->roll);
  }

  void Movement::Move(vec3 target)
  {
    this->position += target;

    this->normalMatrix[0][3] = this->position.x;
    this->normalMatrix[1][3] = this->position.y;
    this->normalMatrix[2][3] = this->position.z;
  }

  void Movement::MoveTo(vec3 target)
  {
    this->position = target;
    this->normalMatrix[0][3] = this->position.x;
    this->normalMatrix[1][3] = this->position.y;
    this->normalMatrix[2][3] = this->position.z;
  }
}