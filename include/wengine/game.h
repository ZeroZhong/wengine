#pragma once

// std

#include <queue>

// local
#include "wengine/texture.h"
#include "wengine/shader.h"
#include "wengine/resource_manager.h"
#include "wengine/render.h"
#include "wengine/landspace.h"
#include "wengine/game_instance.h"
#include "wengine/game_event.h"
#include "wengine/text_render.h"

// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 命名空间和对象
using glm::ortho;

namespace WEngine
{
  class Game
  {
  private:
  public:
    bool Keys[1024];
    bool KeysProcessed[1024];
    GameEvent gameEvent;
    std::queue<GameEventData> eventQueue;
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height);

    void Init();
    void ProcessEvent();
    void Update();
    void Render();
  };

  Game::Game(unsigned int width = 800, unsigned int height = 600)
      : Width(width), Height(height)
  {
    GameInstance::InitGlobalCamera();
    /***
// 1. 加载着色代码
ResourceManager::LoadShader(FileSystem::getPath("planet.vs").c_str(),
                            FileSystem::getPath("planet.fs").c_str(), nullptr, "planet");
// 1.1 配置着色程序
mat4 projection = ortho(0.0f,
                        static_cast<float>(this->Width), static_cast<float>(this->Height),
                        0.0f, -1.0f, 1.0f);
ResourceManager::GetShader("planet").SetMatrix4("projection", projection);
ResourceManager::GetShader("planet").Use().SetInt("planet", 0);

// 2. 加载纹理
ResourceManager::LoadTexture(FileSystem::getPath("background.jpg").c_str(), false, "background");
// 3. 设置专有的渲染器
WEngine::Render planetRender(ResourceManager::GetShader("planet"));
// planetRender.Draw();
// 4. 配置游戏状态
// 5. 游戏等级
// 6. 游戏声音
***/
  }

  // SphereLandSpace *landSpace;
  // SphereLandSpace *landSpace2;
  PlaneLandSpace *pls;
  TextRender *Text;

  void Game::Init()
  {
    // landSpace = new SphereLandSpace(3.0f);
    // landSpace2 = new SphereLandSpace(2.0f);
    pls = new PlaneLandSpace(800.0f, 800.0f);
    Text = new TextRender(800.0f, 600.0f);
    Text->Load(FileSystem::getPath("/fonts/Antonio-Bold.ttf").c_str(), 32);
  }

  //按键事件，持续，释放
  void Game::ProcessEvent()
  {
    while (!eventQueue.empty())
    {
      GameEventData t = eventQueue.front();
      eventQueue.pop();
      gameEvent.Process(t);
    }
  }

  void Game::Update()
  {
    // printf("%d Update\n", GameInstance::GetThreadId());
    // printf("dx,dy: %f %f\n", GameInstance::deltaX, GameInstance::deltaY);
    // GameInstance::GetGlobalCamera().MoveCamera(GameInstance::deltaX, GameInstance::deltaY);
    // GameInstance::GetGlobalCamera().MoveForword(1.0f * glm::radians(GameInstance::fov));
  }

  void Game::Render()
  {
    // landSpace->MoveTo(glm::translate(mat4(1.0f), glm::vec3(-2.0f, 2.0f, -2.0f)));
    // landSpace->color = glm::vec3(1.0f, 0.3f, 0.8f);
    // landSpace->DrawLandSpace();
    // landSpace2->MoveTo(glm::translate(mat4(1.0f), glm::vec3(2.0f, -2.0f, -2.0f)));
    // landSpace2->color = glm::vec3(0.2f, 0.3f, 0.8f);
    // landSpace2->DrawLandSpace();
    pls->DrawLandSpace();
    Text->RenderText("xyz", 10.0f, 10.0f, 1.0f);
    Text->RenderText("(C) LearnOpenGL.com", 40.0f, 30.0f, 2.0f, glm::vec3(0.3, 0.7f, 0.9f));
  }
}