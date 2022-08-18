#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "wengine/movement.h"

using glm::mat4;

namespace WEngine
{
  class ICamera
  {
  public:
    virtual glm::mat4 ViewMatrix() = 0;
  };

  class Camera : public ICamera
  {
  public:
    Movement movement;

  public:
    Camera() {}
    Camera(glm::vec3 position, glm::vec3 target);

    glm::mat4 ViewMatrix();
    void MoveCamera(float deltaX, float deltaY);
    void MoveCamera(float deltaP, float deltaY, float deltaR);
    void MoveCamera(glm::vec3 target);
    void MoveForword(float target);
    void MoveRight(float target);
  };

  Camera::Camera(glm::vec3 position, glm::vec3 target)
  {
    this->movement.SetPosition(position);
    if (target != position)
    {
      glm::vec3 direction = target - position;
      glm::vec3 d = this->movement.direction;
      float p = 0, y = 0, r = 0;
      bool b = (direction.x * d.y == d.x * direction.y &&
                direction.x * d.z == d.x * direction.z &&
                direction.y * d.z == d.y * direction.z);
      bool c = (direction.x * d.y < 0 || direction.x * d.z < 0 || direction.y * d.z < 0);
      if (!b)
      { // 不共线
        y = atan(direction.x / direction.z);
        p = atan(direction.y / glm::sqrt(direction.x * direction.x + direction.z * direction.z));
      }
      else if (c)
      {
        p = 180.0f;
      }

      this->movement.SetAngles(p, y, r);
    }
  }

  glm::mat4 Camera::ViewMatrix()
  {
    return glm::lookAt(this->movement.position, this->movement.direction, this->movement.up);
  }

  void Camera::MoveCamera(float deltaX, float deltaY)
  {
    float dP = deltaY * 0.1f;
    float dY = deltaX * 0.1f;
    this->movement.SetAngles(dP, dY, 0);
    this->movement.PrintMatrix();
  }
  void Camera::MoveCamera(float deltaP, float deltaY, float deltaR)
  {
    this->movement.SetAngles(deltaP, deltaY, deltaR);
  }

  void Camera::MoveCamera(vec3 target)
  {
    this->movement.position += target;
    this->movement.SetPosition(this->movement.position);
  }

  void Camera::MoveForword(float target)
  {
    vec3 t = target * this->movement.direction;
    MoveCamera(t);
    vec3 x = glm::normalize(this->movement.right * this->movement.direction);
    vec3 y = this->movement.right;
    std::cout << " dr : " << x.x << "," << x.y << "," << x.z << " " << sqrt(x.x * x.x + x.y * x.y + x.z * x.z) << std::endl;
    std::cout << " dr : " << y.x << "," << y.y << "," << y.z << " " << sqrt(y.x * y.x + y.y * y.y + y.z * y.z) << std::endl;
    // std::cout << " ru : " << this->movement.right * this->movement.up << std::endl;
    // std::cout << " ud : " << this->movement.up * this->movement.direction << std::endl;
  }
  void Camera::MoveRight(float target)
  {
    vec3 t = target * this->movement.right;
    MoveCamera(t);
  }
}