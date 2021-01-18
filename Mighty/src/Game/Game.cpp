#include <SDL.h>
#include <SDL_image.h>
#include <glm.hpp>

#include "Game.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"

Game::Game() {
  Logger::Log("Constructor called");
  isRunning = false;

  // Initialize member vars
  renderer = nullptr;
  window = nullptr;
  windowHeight = 0;
  windowWidth = 0;
}

Game::~Game() {
  Logger::Log("Destructor called");
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("Error initializing SDL.");
    return;
  }

  // Get window dimensions
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = displayMode.w; // 800
  windowHeight = displayMode.h; // 600

  window = SDL_CreateWindow(
    "Mighty Game Engine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    windowWidth,
    windowHeight,
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    Logger::Err("Error creating SDL window.");
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    Logger::Err("Error creating SDL renderer.");
    return;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::Setup() {
  // TODO:
  // Entity tank = registry.CreateEntity();
  // tank.AddComponent<TransformComponent>();
  // tank.AddComponent<BoxColliderComponent>();
  // tank.AddComponent<SpriteComponent>("./assets/images/tank.png");
}

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
      case SDL_QUIT:
        isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        break;
    }
  }
}

void Game::Update() {
  // Limit FPS (for now)
  int timeToWait = MILLISECONDS_PER_FRAME - SDL_GetTicks() + millisecondsPreviousFrame;
  if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  double deltaTime = ((double)SDL_GetTicks() - (double)millisecondsPreviousFrame) / 1000.0;
  millisecondsPreviousFrame = SDL_GetTicks();

  // TODO:
  // MovementSystem.Update();
  // CollisionSystem.Update();
  // DamageSystem.Update();
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // TODO: Render game objects

  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
