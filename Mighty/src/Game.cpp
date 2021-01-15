#include <iostream>

#include <SDL.h>

#include "Game.h"

Game::Game() {
	std::cout << "Constructor called" << std::endl;
	isRunning = false;
}

Game::~Game() {
	std::cout << "Destructor called" << std::endl;
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	// Get window dimensions
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 800; // displayMode.w;
	windowHeight = 600; // displayMode.h;

	window = SDL_CreateWindow(
		"Mighty Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::Run() {
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
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	// TODO: Render game entities

	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}