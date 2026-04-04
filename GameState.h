#pragma once

#include "Timer.h"
#include "GameObject.h"
#include "Resources.h"
#include <random>
#include "AI.h"


struct ConfiguratedKeys {
	SDL_Scancode rotateCW;
	SDL_Scancode rotateCCW;
	SDL_Scancode rotate180;
	SDL_Scancode moveRight;
	SDL_Scancode moveLeft;

	std::array<SDL_Scancode, 5> handlingKeys;

	SDL_Scancode softDrop;
	SDL_Scancode instantSoftDrop;
	SDL_Scancode hardDrop;
	SDL_Scancode reserve;

	ConfiguratedKeys()
	{
		rotateCW = SDL_SCANCODE_D;
		rotateCCW = SDL_SCANCODE_C;
		rotate180 = SDL_SCANCODE_A;
		moveRight = SDL_SCANCODE_L;
		moveLeft = SDL_SCANCODE_J;


		handlingKeys =
		{
			rotateCW,
			rotateCCW,
			rotate180,
			moveRight,
			moveLeft
		};

		softDrop = SDL_SCANCODE_B;
		instantSoftDrop = SDL_SCANCODE_I;
		hardDrop = SDL_SCANCODE_S;
		reserve = SDL_SCANCODE_K;
	}
};

struct SDLState
{
	SDL_Window* window;
	SDL_GLContext context;
	int width, height, logW, logH;
	const bool* keys;
	bool successfullyInitialized;

	SDLState(int w, int h, int logicalW, int logicalH, SDL_Window* win, SDL_GLContext ctxt, bool initSuccess) :
		keys(SDL_GetKeyboardState(nullptr)),
		width(w),
		height(h),
		logW(logicalW),
		logH(logicalH),
		window(win),
		context(ctxt),
		successfullyInitialized(initSuccess)
	{
	}
};

struct GameState
{
	ConfiguratedKeys configuratedKeys;

	std::vector<LevelData> generalObjects;
	std::vector<GridData> grids;
	std::vector<AIPlayer> IAs;


	unsigned int numOfGrids;

	bool flipBrickCWpressed;    // Clockwise
	bool flipBrickCCWpressed;   // Counter Clockwise
	bool flipBrick180pressed;   // 180 degrees
	bool hardDropPressed;

	bool coutingLockIn;

	Timer startMovTimer;
	Timer repeatedMovTimer;
	Timer gravityTimer;
	Timer gameGravityTimer;
	Timer fallLockInTimer;
	Timer preventAcidentalHardDropTimer;
	Timer FPSTimer;

	int fps;

	std::mt19937 rng;

	std::vector<GameObject> scenarioObjects;

	GameState(Resources& res);
};