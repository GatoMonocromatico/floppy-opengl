#include "game/GameState.h"
#include "game/brickUtils.h"

GameState::GameState(Resources& res) :
	rng(std::random_device{}()),
	flipBrickCWpressed(false),
	flipBrickCCWpressed(false),
	flipBrick180pressed(false),
	hardDropPressed(false),
	coutingLockIn(false),
	numOfGrids(2),
	startMovTimer(Timer(0.15f)),
	repeatedMovTimer(Timer(0.03f, true)),
	gravityTimer(Timer(1.0f)),
	gameGravityTimer(Timer(1.0f)),
	fallLockInTimer(Timer(1.0f)),
	preventAcidentalHardDropTimer(Timer(0.1f, true)),
	FPSTimer(Timer(0.1f)),
	configuratedKeys(ConfiguratedKeys()),
	fps(0)
{
	// creates grids
	for (size_t i = 0; i < numOfGrids; i++)
	{
		grids.push_back(GridData());

		for (size_t j = 0; j < 4; ++j)
		{
			grids[i].previewBrick.units.push_back(GameObject(ObjectType::brickUnit, createUnit(grids.back()), 2));
		}

		for (size_t k = 0; k < 7; k++)
		{
			// fucked type dist <- number from 0 to the amount of bricks in build
			std::uniform_int_distribution<int> dist(0, grids[i].brickBuild.size() - 1);

			//adds the brick referenced in the grid's build by the dist number to the grid's next bricks
			grids[i].nextBricks.push_back(grids[i].brickBuild[dist(rng)]);
		}
	}

	// creates AIdatas
	for (size_t i = 1; i < numOfGrids; i++)
	{
		IAs.push_back(AIPlayer(grids[i], Timer(0.5f), 4, 2));
	}
	
	scenarioObjects.push_back(GameObject(ObjectType::level, 222, 0));
	scenarioObjects.push_back(GameObject(ObjectType::level, 222, 2, res.anims[0], 0));
}