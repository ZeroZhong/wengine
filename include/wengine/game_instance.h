#pragma once

#include <queue>
#include <thread>
// #ifndef __linux__
// #include "windows.h"
// #else
// #include "unistd.h"
// #endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "wengine/camera.h"
// #include "wengine/game_event.h"

namespace WEngine
{
  class GameInstance
  {
  public:
    // 1. 全局相机 每个对象需要全局相机产生的坐标
    static Camera camera;
    // 2. 向上的向量
    static glm::vec3 wordUp;
    static float fov;
    static float deltaX;
    static float deltaY;

    // static std::queue<GameEventData> gameEvents;

    // 1. 全局相机 每个对象需要全局相机产生的坐标
    static Camera &GetGlobalCamera();
    static void InitGlobalCamera();
    static unsigned int GetThreadId();
    // static void PutEvent(int key);
    // static GameEventData GetEvent();

  private:
    GameInstance();
  };
#include <windows.h>

  unsigned int GameInstance::GetThreadId()
  {
    unsigned int pid, tid;
    // tid = std::this_thread::get_id();
    // #ifndef __linux__
    pid = GetCurrentProcessId();
    tid = GetCurrentThreadId();
    // #else
    //     pid = getpid();
    // std::thread::id this_thread_id = std::this_thread::get_id();

    // tid = this_thread_id;
    // std::cout << "this thread id = " << this_thread_id << std::endl;

    // #endif
    // printf("now <pid,tid> is  <%d, %d>  \n", pid, tid);
    return tid;
  }
  GameInstance::GameInstance()
  {
  }
  Camera GameInstance::camera;
  glm::vec3 GameInstance::wordUp;
  float GameInstance::fov;
  float GameInstance::deltaX;
  float GameInstance::deltaY;
  void GameInstance::InitGlobalCamera()
  {
    camera = Camera(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0, 0, 0));
    fov = 0.0f;
    wordUp = glm::vec3(0, 1.0f, 0);
    deltaX = 0;
    deltaY = 0;
  }
  Camera &GameInstance::GetGlobalCamera()
  {
    return camera;
  }

  // std::queue<GameEventData> GameInstance::gameEvents;
  // void GameInstance::PutEvent(int key)
  // {
  //   // GameEventData data = {"key", key};
  //   // gameEvents.push(data);
  // }

  // GameEventData GameInstance::GetEvent()
  // {
  //   // if (gameEvents.size() > 0)
  //   // {
  //   //   GameEventData res = gameEvents.front();
  //   //   gameEvents.pop();
  //   //   return res;
  //   // }
  //   return {};
  // }

} // namespace WEngine
