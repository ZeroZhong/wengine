#pragma once

#include <iostream>
#include <functional>
#include <map>

// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "wengine/game_instance.h"

using std::function;
using std::map;
using std::string;

namespace WEngine
{
  // w | w_press | w_press_event
  // x | w_press | w_press_event
  enum EventType
  {
    KEYBOARD = 0, // 键盘
    MOUSE = 1     // 鼠标
  };
  struct GameEventData
  {
    int type; // 键盘，鼠标
    int key;
    int action;
  };
  using fptr = void(GameEventData);
  class GameEvent
  {
  public:
    string name; // 事件名
    int type;    // 事件类型
    map<int, function<fptr>> funcs;
    map<int, string> eventMap = {
        {GLFW_KEY_W, "w"},
        {GLFW_KEY_S, "s"},
        {GLFW_KEY_A, "a"},
        {GLFW_KEY_D, "d"}};
    GameEvent();
    void Process(GameEventData data);
    void Init();
  };

  GameEvent::GameEvent()
  {
    Init();
  }

  void GameEvent::Init()
  {
    funcs.insert({GLFW_KEY_W, [](GameEventData t)
                  { GameInstance::GetGlobalCamera().MoveForword(1.0f); }});
    funcs.insert({GLFW_KEY_S, [](GameEventData t)
                  { GameInstance::GetGlobalCamera().MoveForword(-1.0f); }});
    funcs.insert({GLFW_KEY_A, [](GameEventData t)
                  { GameInstance::GetGlobalCamera().MoveRight(1.0f); }});
    funcs.insert({GLFW_KEY_D, [](GameEventData t)
                  { GameInstance::GetGlobalCamera().MoveRight(-1.0f); }});
  }

  void GameEvent::Process(GameEventData data)
  {
    try
    {
      int key = data.key;
      funcs[key](data);
    }
    catch (const std::bad_function_call &gfg)
    {
      std::cout << "GameEvent Process Error : " << gfg.what() << std::endl;
    }
  }

} // namespace WEngine
