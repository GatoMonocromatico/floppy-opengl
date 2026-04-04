#pragma once
#include "GameState.h"
#include "GameObject.h"
#include "brickUtils.h"




struct AtackInfo
{
	int atack;
	std::vector<std::vector<GameObject>> unitDataRowsEliminated;
	std::vector<int8_t> rowsEliminatedCords;
	int8_t lowestRowEliminated;
	int8_t highestRowEliminated;
	AtackInfo(int atk, std::vector<std::vector<GameObject>> unitDatas, std::vector<int8_t> Ycords, int8_t lowestRow) :
		atack(atk),
		rowsEliminatedCords(Ycords),
		unitDataRowsEliminated(unitDatas),
		lowestRowEliminated(lowestRow)
	{
	}
	AtackInfo() : atack(0)
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

void createPlayingBrick(GameState& gs, Resources& res, std::vector<BrickData>& currentBricks, size_t thisGridIndex, Piece shape);
void updateBricks(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, BrickData& brick, size_t thisGridIndex, float deltaTime);
AtackInfo playingPieceDropped(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks = true);
void stepNextBricks(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex);
void eraseBricksFromCurrentBricks(GridData& gridData, std::vector<int16_t>& indexesOfDeprecatedBricks);
AtackInfo cleanFullLines(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex);
void updateGrid(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks);
