
# 问题

1. 注意64位和32位的区别
  `skipping incompatible lib/libglfw3dll.a when searching for -lglfw3dll`

# 代码

```cpp
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include <queue>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/**
 * @brief 划分球面
 *
 */
struct XYZ
{
  GLdouble x, y, z;
};

XYZ midArcPoint(const XYZ &a, const XYZ &b)
{
  XYZ c{a.x + b.x,
   a.y + b.y,
    a.z + b.z};
  GLdouble mod = sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
  c.x /= mod, c.y /= mod, c.z /= mod;
  return c;
}

GLdouble distSquare(const XYZ &a, const XYZ &b)
{
  GLdouble dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
  return dx * dx + dy * dy + dz * dz;
}

/**
 * @brief 面细分法
 * @param resolution 分辨率
 */
void DrawSphere_2(GLdouble resolution)
{
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  struct Triangle
  {
    XYZ a, b, c;
  };

  std::queue<Triangle> triangles;
  triangles.push(
    {1, 0, 0,
   0, 1, 0,
   0, 0, 1});

  resolution *= resolution;
  while (!triangles.empty())
  {
    auto &t = triangles.front();

    // 当三角形各边长度都不大于resolution时就不再进一步细分
    if (distSquare(t.a, t.b) > resolution ||
        distSquare(t.b, t.c) > resolution ||
        distSquare(t.c, t.a) > resolution)
    {
      auto d = midArcPoint(t.a, t.b),
           e = midArcPoint(t.b, t.c),
           f = midArcPoint(t.c, t.a);
      triangles.push({t.a, f, d});
      triangles.push({t.b, d, e});
      triangles.push({t.c, e, f});
      triangles.push({d, e, f});
    }
    else
    {
      glBegin(GL_TRIANGLES);

      // 第一象限
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第二象限
      t.a.x = -t.a.x, t.b.x = -t.b.x, t.c.x = -t.c.x;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第三象限
      t.a.y = -t.a.y, t.b.y = -t.b.y, t.c.y = -t.c.y;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第四象限
      t.a.x = -t.a.x, t.b.x = -t.b.x, t.c.x = -t.c.x;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第五象限
      t.a.z = -t.a.z, t.b.z = -t.b.z, t.c.z = -t.c.z;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第六象限
      t.a.x = -t.a.x, t.b.x = -t.b.x, t.c.x = -t.c.x;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第七象限
      t.a.y = -t.a.y, t.b.y = -t.b.y, t.c.y = -t.c.y;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);

      // 第八象限
      t.a.x = -t.a.x, t.b.x = -t.b.x, t.c.x = -t.c.x;
      glVertex3dv((GLdouble *)&t.a);
      glVertex3dv((GLdouble *)&t.b);
      glVertex3dv((GLdouble *)&t.c);
      glEnd();
    }
    triangles.pop();
  }
}

/**
 * @brief
 *
 */
/**
 * @brief 扎篮子法
 * @param resolution 分辨率
 */
const GLdouble kPi = 1000.0;
void DrawSphere_1(GLuint resolution)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLdouble dTheta = kPi / resolution;

  struct XY
  {
    GLdouble x, y;
  };
  auto a = new XY[2 * resolution + 1];
  auto b = new XY[2 * resolution + 1];

  GLdouble thetaZ = dTheta;  // 纬转角
  GLdouble thetaXY = 0;      // 经转角
  GLdouble az = cos(thetaZ); // a纬面高
  GLdouble bz;               // b纬面高
  GLdouble rr = sin(thetaZ); // 纬半径

  // 绘制顶盖
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0, 0, 1);
  for (auto i = 0U; i < 2 * resolution; ++i)
  {
    a[i].x = cos(thetaXY) * rr;
    a[i].y = sin(thetaXY) * rr;
    thetaXY += dTheta;
    glVertex3d(a[i].x, a[i].y, az);
  }
  a[2 * resolution] = a[0];
  glVertex3d(a[2 * resolution].x, a[2 * resolution].y, az); // 封闭纬面
  glEnd();

  // 绘制中间层
  for (auto i = 1U; i < resolution - 1; ++i)
  {
    thetaZ += dTheta;
    bz = cos(thetaZ) * 1;
    rr = sin(thetaZ) * 1;
    thetaXY = dTheta / 2 * i;

    glBegin(GL_TRIANGLES);
    for (auto j = 0U; j < 2 * resolution; ++j)
    {
      b[j].x = cos(thetaXY) * rr;
      b[j].y = sin(thetaXY) * rr;
      thetaXY += dTheta;

      glVertex3d(a[j].x, a[j].y, az);
      glVertex3d(a[j + 1].x, a[j + 1].y, az);
      glVertex3d(b[j].x, b[j].y, bz);
    }
    b[2 * resolution] = b[0];
    glEnd();

    auto tmp = a;
    a = b;
    b = tmp;
    az = bz;
  }

  // 绘制底盖
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0, 0, -1);
  for (auto i = 0U; i < 2 * resolution; ++i)
  {
    glVertex3d(a[i].x, a[i].y, az);
  }
  glVertex3d(a[2 * resolution].x, a[2 * resolution].y, az); // 封闭纬面
  glEnd();

  delete[] a;
  delete[] b;
}

/**
 * @brief demo
 *
 * @return int
 */
int demo()
{
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: 当窗口大小发生变化，调用这个函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // 创建一个视口
  glViewport(0, 0, width, height);
}

void display()
{

  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 0.0, 0.0);

  glBegin(GL_POLYGON);

  glVertex2f(120, 100);

  glVertex2f(280, 100);

  glVertex2f(150, 240);

  glEnd();

  glFlush();
}

const int screen_width = 800;
const int screen_hight = 600;

//三角形的顶点数据
const float triangle[] = {
    //--位置--//
    -0.5f, -0.5f, 0.0f, //左下
    0.5f, -0.5f, 0.0f,  //右下
    0.0f, 0.5f, 0.0f,   //正上
};

int demo2()
{

  //初始化glfw
  glfwInit();                                                    //初始化glfw
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 //设置主版本号
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 //设置次版本号
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置核心模式
  glfwWindowHint(GLFW_RESIZABLE, false);                         //关闭可调节窗口大小

  //创建窗口(宽、高、窗口名)
  auto window = glfwCreateWindow(screen_width, screen_hight, "triangle", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "failed to create opengl context" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  //初始化glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "failed to initialize glad" << std::endl;
    return -1;
  }

  //创建视口
  glViewport(0, 0, screen_width, screen_hight);

  //生成并绑定vbo
  GLuint vertex_buffer_object;
  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  //将顶点数据绑定至默认的缓冲中
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
  //生成并绑定vao
  GLuint vertex_array_object;
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  //设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(float), (void *)0); //参数：顶点着色器位置值，分量，顶点数据类型，是否标准化，步长、数据偏移量
  glEnableVertexAttribArray(0);                        //开启0通道

  //设置完成之后就可以解绑VBO、VAO了
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //顶点着色器源码
  const char *vertex_shader_source =
      "#version 330 core\n"
      "layout (location = 0) in vec3 apos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(apos,1.0);\n"
      "}\n\0";

  //片段着色器源码
  const char *fragment_shader_source =
      "#version 330 core\n"
      "out vec4 fragcolor;\n"
      "void main()\n"
      "{\n"
      "   fragcolor = vec4(1.0f,0.1f,0.1f,1.0f);\n"
      "}\n\0";

  //生成并编译着色器
  //顶点着色器
  int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  // 检查着色器是否成功编译，如果编译失败，打印错误信息
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    std::cout << "error::shader::vertex::compilation_failed\n"
              << info_log << std::endl;
  }
  // 片段着色器
  int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  // 检查着色器是否成功编译，如果编译失败，打印错误信息
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cout << "error::shader::fragment::compilation_failed\n"
              << info_log << std::endl;
  }
  // 链接顶点和片段着色器至一个着色器程序
  int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  // 检查着色器是否成功链接，如果链接失败，打印错误信息
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cout << "error::shader::program::linking_failed\n"
              << info_log << std::endl;
  }
  // 删除着色器
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // 线框模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //渲染循环
  while (!glfwWindowShouldClose(window))
  {

    // 清空颜色缓冲
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 使用着色器程序
    glUseProgram(shader_program);

    // 绘制三角形
    glBindVertexArray(vertex_array_object); // 绑定VAO

    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
    // DrawSphere_2(1);
    glBindVertexArray(0); // 解除绑定

    // 交换缓冲并且检查是否有触发事件(比如键盘输入、鼠标移动等）
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 删除vao和vbo
  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);

  // 清理所有的资源并正确退出程序
  glfwTerminate();
  return 0;
}

int main(int argc, char **argv)
{

  demo2();
  return 0;
}
```

# 基本代码设计

## 主流程 main.cpp

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"

#include <iostream>

// 回调函数
// 窗口大小调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 按键回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// 屏幕宽高
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// 游戏核心
Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
#pragma region OpenGL 初始化和配置
    //1. OpenGL 初始化
    //1.1 OpenGL 初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    //1.2 OpenGL 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    //1.3 OpenGL 加载OpenGL函数库
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //2. OpenGL 配置
    //2.1 OpenGL 设置回调函数
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //2.2 OpenGL 配置
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion

#pragma region 游戏核心
    // 1. 游戏正式初始化
    Breakout.Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // 2. 游戏核心循环
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //OpenGL 检查事件
        glfwPollEvents();

        //2.1. 处理输入事件
        Breakout.ProcessInput(deltaTime);
        //2.2. 更新游戏状态
        Breakout.Update(deltaTime);

        //OpenGL 设置背景色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //2.3 渲染游戏画面
        Breakout.Render();

        //OpenGL 交换缓冲区
        glfwSwapBuffers(window);
    }
    //3. 回收游戏资源
    ResourceManager::Clear();
#pragma endregion
    //关闭OpenGL
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // ESC -> 退出
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 把按键传递给游戏核心类处理
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = false;
            Breakout.KeysProcessed[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 显示视口
    glViewport(0, 0, width, height);
}
```

## 游戏核心类 Game.h

```cpp
class Game
{
public:
    // 游戏状态  : GAME_ACTIVE,  GAME_MENU,  GAME_WIN
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    Game(unsigned int width, unsigned int height);
    ~Game();
    // 初始化游戏状态 (加载所有的 shaders/textures/levels)
    void Init();
    // 游戏循环
    // 事件处理
    void ProcessInput(float dt);
    // 更新游戏状态
    void Update(float dt);
    // 画面渲染
    void Render();
};

// 游戏相关的数据
SpriteRenderer    *Renderer;
GameObject        *Player;
BallObject        *Ball;
ParticleGenerator *Particles;
PostProcessor     *Effects; // 游戏进度监测点
ISoundEngine      *SoundEngine = createIrrKlangDevice();

void Game::Update(float dt)
{
    // 更新对象
    Ball->Move(dt, this->Width);
     // 碰撞监测
    this->DoCollisions();
    // update particles
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
    // update PowerUps
    this->UpdatePowerUps(dt);
    // reduce shake time
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
    // 结束监测
    if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}
```

## 资源管理器 ResourceManager.h

```cpp
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char *file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // properly de-allocates all loaded resources
    static void      Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};
```

## 着色器 Shader.h

```cpp
class Shader
{
public:
    // state
    unsigned int ID;
    // constructor
    Shader() { }
    // sets the current shader as active
    Shader  &Use();
    // compiles the shader from given source code
    void    Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr); // note: geometry source code is optional
    // utility functions
    void    SetFloat    (const char *name, float value, bool useShader = false);
    void    SetInteger  (const char *name, int value, bool useShader = false);
    void    SetVector2f (const char *name, float x, float y, bool useShader = false);
    void    SetVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void    SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
    void    SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void    SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
    void    SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void    SetMatrix4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
private:
    // checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(unsigned int object, std::string type);
};
```

## 纹理 Texture2D.h

```cpp
class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int ID;
    // texture image dimensions
    unsigned int Width, Height; // width and height of loaded image in pixels
    // texture Format
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format; // format of loaded image
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};
```

## 游戏角色对象 GameObject.h

```cpp
class GameObject
{
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};
```

## 游戏角色对象渲染器 SpriteRenderer.h

```cpp
class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    // Render state
    Shader       shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // 准备着色器
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    // render textured quad
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
```

## 资源

### 着色程序

sprite.vs

```glsl
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
// note that we're omitting the view matrix; the view never changes so we basically have an identity view matrix and can therefore omit it.
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
```

sprite.fs

```glsl
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}
```

# 参考

- [vsc 搭建opengl 开发环境](https://blog.csdn.net/weixin_43952192/article/details/122877840)
- [问题:skipping incompatible lib/libglfw3dll.a when searching for -lglfw3dll](https://blog.csdn.net/caiweibin2246/article/details/114885843)
- [球面三角网格绘制算法（附OpenGL代码）](https://blog.csdn.net/u014132143/article/details/111761448)
- [OpenGL 多边形用glPolygonMode设置绘制模式](https://blog.csdn.net/Meloor/article/details/102937631)
- [glut太旧，现在使用glfw替代](http://www.wjhsh.net/MenAngel-p-5640133.html)