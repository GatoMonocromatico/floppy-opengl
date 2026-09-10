#pragma once
#include "util/StdAliases.h"
#include "game/GameState.h"
#include "game/gameObject.h"
#include "game/brickUtils.h"




// What a cleared cell was, captured before its unit is released back to the pool.
// Storing handles here would dangle -- the units are freed in the same frame. This is
// the data a clear animation (and later, garbage materialisation) actually needs.
struct ClearedUnit
{
	glm::ivec2 position;
	Piece shape;
};

struct AtackInfo
{
	int atack;
	stdMat<ClearedUnit> unitsEliminated;
	std::vector<int8_t> rowsEliminatedCords;
	int8_t lowestRowEliminated;
	int8_t highestRowEliminated;

	AtackInfo() : atack(0), lowestRowEliminated(-1), highestRowEliminated(-1)
	{
	}

	explicit operator bool() const
	{
		return atack > 0;
	}
	bool rowsWereEliminated() const
	{
		return rowsEliminatedCords.size() > 0;
	}
};

void createPlayingBrick(GameState& gs, Resources& res, size_t thisGridIndex, Piece shape);
std::array<bool, 3> updatePlayingBrick(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, float deltaTime);
AtackInfo playingPieceDropped(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks = true);
void stepNextBricks(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex);
AtackInfo cleanFullLines(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex);
void updateGrid(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks);
