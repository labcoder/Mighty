#pragma once

#include <memory>
#include <SDL.h>
#include "../ECS/ECS.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool isRunning;
  int millisecondsPreviousFrame = 0;
  SDL_Window* window;
  SDL_Renderer* renderer;

  std::unique_ptr<Registry> registry;

public:
  Game();
  ~Game();
  void Initialize();
  void Setup();
  void Run();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  int windowWidth;
  int windowHeight;
};
