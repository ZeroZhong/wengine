// std
#include <iostream>

// local
#include "wengine/shader.h"
#include "wengine/model.h"
#include "wengine/camera.h"
#include "wengine/landspace.h"
#include "wengine/game.h"
#include "wengine/game_instance.h"
// #include "wengine/game_event.h"

// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace WEngine;
using std::string;

// 声明OpenGL回调事件
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

GLFWwindow *InitOpenGL(unsigned int width, unsigned int height, string &gameName)
{
#pragma region OpenGL : 初始化和配置
  // 1. 初始化, 窗口，配置，上下文，标志等
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, gameName.c_str(), NULL, NULL);
  glfwMakeContextCurrent(window);

  // 初始化glad
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#pragma endregion

#pragma region OpenGL : 配置
  //设置回调函数
  //窗口改变
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //键盘事件
  glfwSetKeyCallback(window, key_callback);
  //设置输入模式(关闭光标)
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //鼠标位置事件
  glfwSetCursorPosCallback(window, mouse_callback);
  //鼠标滚轮事件
  glfwSetScrollCallback(window, scroll_callback);

  // 显示窗口
  glViewport(0, 0, width, height);
  // 开启剔除面
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  // 开启混合，这样文字才能显示
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // 线框模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion
  return window;
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
string GAME_NAME = "MyGame";
Game game(SCR_WIDTH, SCR_HEIGHT);

int main1(int argc, char **argv)
{
  std::cout << "size : " << sizeof({}) << std::endl;
  return 0;
}
int main(int argc, char **argv)
{
  // OpenGL初始化
  GLFWwindow *window = InitOpenGL(SCR_WIDTH, SCR_HEIGHT, GAME_NAME);

  // 游戏核心初始化
  game.Init();

  while (!glfwWindowShouldClose(window))
  {
    // OpenGL事件检测
    glfwPollEvents();
    // 游戏事件处理
    game.ProcessEvent();
    // 更新游戏状态
    game.Update();

    // OpenGL重置着色程序
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 游戏画面渲染
    game.Render();

    // OpenGL交换缓冲
    glfwSwapBuffers(window);
  }
  // 关闭OpenGL
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  // printf("%d : int key, int scancode, int action, int mode  (%d ,%d,%d ,%d)\n", GameInstance::GetThreadId(), key, scancode, action, mode);
  // ESC -> 退出
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
    return;
  }
  // 把按键传递给游戏核心类处理
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      game.eventQueue.push({EventType::KEYBOARD, key, action});
  }
}

float lastXPos, lastYPos;
bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
  if (firstMouse)
  {
    lastXPos = xPos;
    lastYPos = yPos;
    firstMouse = false;
  }

  float deltaX, deltaY;
  deltaX = xPos - lastXPos;
  deltaY = yPos - lastYPos;
  lastXPos = xPos;
  lastYPos = yPos;
  GameInstance::deltaX = deltaX;
  GameInstance::deltaY = deltaY;
  // GameInstance::GetGlobalCamera().MoveCamera(deltaX, deltaY);
  // printf("%d , %d (%f dx,%f dy)\n", GameInstance::GetThreadId(), firstMouse, deltaX, deltaY);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  // printf("xoffset yoffset %lf %lf\n", xoffset, yoffset);
  // float fov = GameInstance::fov;
  // if (fov >= 1.0f && fov <= 45.0f)
  // {
  //   fov -= yoffset;
  // }
  // else if (fov <= 1.0f)
  // {
  //   fov = 1.0f;
  // }
  // else if (fov >= 45.0f)
  // {
  //   fov = 45.0f;
  // }
  GameInstance::fov += yoffset;
  if (GameInstance::fov < 0)
  {
    GameInstance::fov = 0;
  }
}