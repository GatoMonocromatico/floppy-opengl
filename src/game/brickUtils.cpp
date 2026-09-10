#include "game/brickUtils.h"
#include "util/DebugLog.h"

std::array<glm::ivec2, 4> createStartingPos(Piece s)
{
	switch (s)
	{
	case Piece::T:
		return {
			glm::ivec2(3.0f, 4.0f),
			glm::ivec2(4.0f, 3.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 4.0f)
		};
		break;
	case Piece::S:
		return {
			glm::ivec2(3.0f, 4.0f),
			glm::ivec2(4.0f, 3.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 3.0f)
		};
		break;

	case Piece::Z:
		return {
			glm::ivec2(3.0f, 3.0f),
			glm::ivec2(4.0f, 3.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 4.0f)
		};
		break;
	case Piece::J:
		return {
			glm::ivec2(3.0f, 3.0f),
			glm::ivec2(3.0f, 4.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 4.0f)
		};
		break;
	case Piece::L:
		return {
			glm::ivec2(3.0f, 4.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 3.0f),
			glm::ivec2(5.0f, 4.0f)
		};
		break;
	case Piece::I:
		return {
			glm::ivec2(3.0f, 4.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 4.0f),
			glm::ivec2(6.0f, 4.0f)
		};
		break;
	case Piece::O:
		return {
			glm::ivec2(4.0f, 3.0f),
			glm::ivec2(4.0f, 4.0f),
			glm::ivec2(5.0f, 3.0f),
			glm::ivec2(5.0f, 4.0f)
		};
		break;
	default:
		return {
			glm::ivec2(0, 0),
			glm::ivec2(0, 0),
			glm::ivec2(0, 0),
			glm::ivec2(0, 0)
		};
		DBG_IF(flux::verbose, "INVALID PIECE DETECTED!!!!!!!!!!!");
		break;
	}
}

stdArrMat<glm::ivec2, 4, 4> createRotationalAdjustments(Piece s) {
	switch (s)
	{
	case Piece::T:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0), glm::ivec2(0), glm::ivec2(0), glm::ivec2(0)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f)}
		};
		break;
	case Piece::S:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0), glm::ivec2(0), glm::ivec2(0), glm::ivec2(0)},
			std::array<glm::ivec2, 4> { glm::ivec2(0.0f, 2.0f), glm::ivec2(-2.0f, 0.0f), glm::ivec2(0.0f, -2.0f), glm::ivec2(2.0f, 0.0f)}
		};
		break;

	case Piece::Z:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(2.0f, 0.0f), glm::ivec2(0.0f, 2.0f), glm::ivec2(-2.0f, 0.0f), glm::ivec2(0.0f, -2.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0), glm::ivec2(0), glm::ivec2(0), glm::ivec2(0)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f)}
		};
		break;
	case Piece::J:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(2.0f, 0.0f), glm::ivec2(0.0f, 2.0f), glm::ivec2(-2.0f, 0.0f), glm::ivec2(0.0f, -2.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0), glm::ivec2(0), glm::ivec2(0), glm::ivec2(0)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f)}
		};
		break;
	case Piece::L:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f), glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0), glm::ivec2(0), glm::ivec2(0), glm::ivec2(0)},
			std::array<glm::ivec2, 4> { glm::ivec2(0.0f, 2.0f), glm::ivec2(-2.0f, 0.0f), glm::ivec2(0.0f, -2.0f), glm::ivec2(2.0f, 0.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 1.0f), glm::ivec2(-1.0f, -1.0f), glm::ivec2(1.0f, -1.0f), glm::ivec2(1.0f, 1.0f)}
		};
		break;
	case Piece::I:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(2.0f, -1.0f), glm::ivec2(1.0f, 2.0f), glm::ivec2(-2.0f, 1.0f), glm::ivec2(-1.0f, -2.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, 0.0f), glm::ivec2(0.0f, 1.0f), glm::ivec2(-1.0f, 0.0f), glm::ivec2(0.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0.0f, 1.0f), glm::ivec2(-1.0f, 0.0f), glm::ivec2(0.0f, -1.0f), glm::ivec2(1.0f, 0.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 2.0f), glm::ivec2(-2.0f, -1.0f), glm::ivec2(1.0f, -2.0f), glm::ivec2(2.0f, 1.0f)}
		};
		break;
	case Piece::O:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(1.0f, 0.0f), glm::ivec2(0.0f, 1.0f), glm::ivec2(-1.0f, 0.0f), glm::ivec2(0.0f, -1.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0.0f, -1.0f), glm::ivec2(1.0f, 0.0f), glm::ivec2(0.0f, 1.0f), glm::ivec2(-1.0f, 0.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(0.0f, 1.0f), glm::ivec2(-1.0f, 0.0f), glm::ivec2(0.0f, -1.0f), glm::ivec2(1.0f, 0.0f)},
			std::array<glm::ivec2, 4> { glm::ivec2(-1.0f, 0.0f), glm::ivec2(0.0f, -1.0f), glm::ivec2(1.0f, 0.0f), glm::ivec2(0.0f, 1.0f)}
		};
		break;
	default:
		return {
			std::array<glm::ivec2, 4> { glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0)},
			std::array<glm::ivec2, 4> { glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0)},
			std::array<glm::ivec2, 4> { glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0)},
			std::array<glm::ivec2, 4> { glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0), glm::ivec2(0, 0)}
		};
		DBG_IF(flux::verbose, "INVALID PIECE DETECTED!!!!!!!!!!!");
		break;
	}
}

// Allocates a slot from the free list when one is available, only growing the pool
// when it is empty. This is what keeps currentUnits bounded -- it used to grow by 4
// every piece forever, because the compaction that was supposed to reclaim slots
// never actually shrank the vector.
static size_t allocUnitSlot(GridData& gridData)
{
	if (!gridData.freeUnits.empty())
	{
		size_t handle = gridData.freeUnits.back();
		gridData.freeUnits.pop_back();
		return handle;
	}

	gridData.currentUnits.push_back(BrickUnitData());
	return gridData.currentUnits.size() - 1;
}

size_t createUnit(GridData& gridData, glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id)
{
	size_t handle = allocUnitSlot(gridData);
	gridData.currentUnits[handle] = BrickUnitData(pos, ra, s, index, id);

	return handle;
}
size_t createUnit(GridData& gridData, int x, int y, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id)
{
	return createUnit(gridData, glm::ivec2(x, y), ra, s, index, id);
}
size_t createUnit(GridData& gridData)
{
	size_t handle = allocUnitSlot(gridData);
	gridData.currentUnits[handle] = BrickUnitData();

	return handle;
}

void freeUnit(GridData& gridData, size_t handle)
{
	// Left in place, only marked dead and queued for reuse. Moving it would
	// invalidate every handle stored in a GridCell or a brick's units.
	gridData.currentUnits[handle] = BrickUnitData();
	gridData.freeUnits.push_back(handle);
}

size_t createBrick(GridData& gridData, Piece shape, uint32_t id)
{
	size_t handle;

	if (!gridData.freeBricks.empty())
	{
		handle = gridData.freeBricks.back();
		gridData.freeBricks.pop_back();
	}
	else
	{
		gridData.currentBricks.push_back(BrickData());
		handle = gridData.currentBricks.size() - 1;
	}

	gridData.currentBricks[handle] = BrickData(shape, static_cast<unsigned int>(handle), id);

	return handle;
}

void freeBrick(GridData& gridData, size_t handle)
{
	gridData.currentBricks[handle] = BrickData();
	gridData.freeBricks.push_back(handle);
}


int getbiggestYFallForBrick(BrickData& brick, GridData& gridData)
{
	DBG_IF(flux::verbose, "CALCULATING BIGGEST FALL");
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	int lowestDeltaY = 25;

	for (GameObject& u : brick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

		int deltaY = 0;

		while (isUnitPositionValid(u, uData.position.x, uData.position.y + deltaY + 1, gridData))
		{
			deltaY += 1;
		}

		if (deltaY < lowestDeltaY)
		{
			lowestDeltaY = deltaY;
		}
	}

	DBG_IF(flux::verbose, "fall", lowestDeltaY);

	return lowestDeltaY;
}


bool isUnitPositionValid(GameObject& u, int x, int y, GridData& gridData)
{
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

	bool validation = true;

	if (x < 0 || x > 9 || y < 0 || y > 25)
	{
		validation = false;
	}
	else if (grid[y][x] && gridData.currentUnits[grid[y][x].unitHandle].indexInCurrentBricks != uData.indexInCurrentBricks)
	{
		validation = false;
		DBG_IF(flux::verbose, "invalid position: already has a brick in position");

	}

	return validation;
};


bool updateBrickPositionTranslational(BrickData& brick, GridData& gridData, glm::ivec2 deltaPos, bool changeData)
{
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	bool isValidMove = true;

	int newX, newY;
	for (GameObject& u : brick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

		newX = uData.position.x + deltaPos.x;
		newY = uData.position.y + deltaPos.y;

		if (!isUnitPositionValid(u, newX, newY, gridData))
		{
			isValidMove = false;
			break;
		}

	}

	if (isValidMove) {

		for (GameObject& u : brick.units)
		{
			BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
			uData.position += deltaPos;
		}

		if (changeData)
		{
			if (!brick.hasToUpdate)
			{
				gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
				brick.hasToUpdate = true;
			}
		}

	}
	return isValidMove;
};

bool updateBrickPositionRotation(BrickData& brick, GridData& gridData, int CWRotations, bool changeData)
{
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	bool isValidMove = true;

	std::array<glm::vec2, 4> liquidRotationalAdjustments;
	liquidRotationalAdjustments.fill(glm::vec2(0));

	glm::vec2 TranslationalAdjustmentFactor(0);

	int newX, newY;

	for (size_t i = 0; i < 4; ++i)
	{
		BrickUnitData& uData = gridData.currentUnits[brick.units[i].specificDataLocation];

		for (int j = 0; j < CWRotations; ++j)
		{
			liquidRotationalAdjustments[i] += uData.rotAdjustments[(uData.rotationState + j) % 4];
		}

		newX = uData.position.x + liquidRotationalAdjustments[i].x;
		newY = uData.position.y + liquidRotationalAdjustments[i].y;

		if (!isUnitPositionValid(brick.units[i], newX, newY, gridData)) {
			isValidMove = false;
		}
	}

	// if plain rotation is not valid
	if (!isValidMove)
	{
		arrVec<glm::ivec2, 3> testAdjustments;

		// sets wall kicks
		if (brick.shape == Piece::I)
		{

			switch (brick.rotationState)
			{
			case 0:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(-2, 0), glm::ivec2(1, 0), glm::ivec2(-2, 1), glm::ivec2(1, -2)},
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 0), glm::ivec2(2, 0), glm::ivec2(0, -1)},
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(2, 0), glm::ivec2(-1, -2), glm::ivec2(2, 1)}
				};
				break;
			case 1:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(2, 0), glm::ivec2(-1, -2), glm::ivec2(2, 1)}, // 1 -> 2
				std::vector<glm::ivec2> { glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(0, 1), glm::ivec2(0, 2), glm::ivec2(-1, 0)}, // 1 -> 3
				std::vector<glm::ivec2> { glm::ivec2(2, 0), glm::ivec2(-1, 0), glm::ivec2(2, -1), glm::ivec2(-1, 2)} // 1 -> 0
				};
				break;
			case 2:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(2, 0), glm::ivec2(-1, 0), glm::ivec2(2, -1), glm::ivec2(-1, 2)}, // 2 -> 3
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(2, 0), glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(0,1)}, // 2 -> 0
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 2), glm::ivec2(-2, -1)} // 2 -> 1
				};
				break;
			case 3:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 2), glm::ivec2(-2, -1)}, // 3 -> 0
				std::vector<glm::ivec2> { glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(0, 1), glm::ivec2(0, 2), glm::ivec2(1, 0)}, // 3 -> 1
				std::vector<glm::ivec2> { glm::ivec2(-2, 0), glm::ivec2(1, 0), glm::ivec2(-2, 1), glm::ivec2(1, -2)} // 3 -> 2
				};
				break;
			}

		}
		else
		{
			switch (brick.rotationState)
			{
			case 0:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-1, -1), glm::ivec2(0, 2), glm::ivec2(-1, 2)}, // 0 -> 1
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(2, 0), glm::ivec2(1, -1), glm::ivec2(2, -1), glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(-1, -1), glm::ivec2(-2, -1), glm::ivec2(0, 1), glm::ivec2(3, 0), glm::ivec2(-3, 0)}, // 0 -> 2
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(1, -1), glm::ivec2(0, 2), glm::ivec2(1, 2)} // 0 -> 3
				};
				break;
			case 1:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(1, 1), glm::ivec2(0, -2), glm::ivec2(1, -2)}, // 1 -> 2
				std::vector<glm::ivec2> { glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(-1, -1), glm::ivec2(-1, -2), glm::ivec2(0, 1), glm::ivec2(0, 2), glm::ivec2(-1, 1), glm::ivec2(-1, 2), glm::ivec2(1, 0), glm::ivec2(0, -3), glm::ivec2(0, 3)}, // 1 -> 3
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(1, 1), glm::ivec2(0, -2), glm::ivec2(1, -2)} // 1 -> 0
				};
				break;
			case 2:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(1, 0), glm::ivec2(1, -1), glm::ivec2(0, 2), glm::ivec2(1, 2)}, // 2 -> 3
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(-1, 1), glm::ivec2(-2, 1), glm::ivec2(1, 0), glm::ivec2(2, 0), glm::ivec2(1, 1), glm::ivec2(2, 1), glm::ivec2(0, -1), glm::ivec2(-3, 0), glm::ivec2(3, 0)}, // 2 -> 0
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-1, -1), glm::ivec2(0, 2), glm::ivec2(-1, 2)} // 2 -> 1
				};
				break;
			case 3:
				testAdjustments =
				{
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-1, 1), glm::ivec2(0, -2), glm::ivec2(-1, -2)}, // 3 -> 0
				std::vector<glm::ivec2> { glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(1, -1), glm::ivec2(1, -2), glm::ivec2(0, 1), glm::ivec2(0, 2), glm::ivec2(1, 1), glm::ivec2(1, 2), glm::ivec2(-1, 0), glm::ivec2(0, -3), glm::ivec2(0, 3)}, // 3 -> 1
				std::vector<glm::ivec2> { glm::ivec2(-1, 0), glm::ivec2(-1, 1), glm::ivec2(0, -2), glm::ivec2(-1, -2)}, // 3 -> 2
				};
				break;
			}

		}

		for (glm::ivec2& testAdjustmentPosVector : testAdjustments[CWRotations - 1]) // CWRotations ranges from 1-3, testAdjustments ranges from 0-2
		{
			for (int i = 0; i < 4; ++i)
			{
				BrickUnitData& uData = gridData.currentUnits[brick.units[i].specificDataLocation];

				newX = uData.position.x + liquidRotationalAdjustments[i].x + testAdjustmentPosVector.x;
				newY = uData.position.y + liquidRotationalAdjustments[i].y + testAdjustmentPosVector.y;
				if (isUnitPositionValid(brick.units[i], newX, newY, gridData)) {
					isValidMove = true;
				}
				else
				{
					isValidMove = false;
					break;
				}

			}
			if (isValidMove)
			{
				TranslationalAdjustmentFactor = testAdjustmentPosVector;
				break;
			}

		}
	}
	
	if (isValidMove)
	{
		brick.rotationState = (brick.rotationState + CWRotations) % 4;

		for (size_t i = 0; i < 4; ++i)
		{
			BrickUnitData& uData = gridData.currentUnits[brick.units[i].specificDataLocation];

			uData.position += liquidRotationalAdjustments[i] + TranslationalAdjustmentFactor;

			uData.rotationState = brick.rotationState;
		}

		if (changeData)
		{
			brick.wasntHandledSinceRotation = true;

			if (!brick.hasToUpdate)
			{
				gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
				brick.hasToUpdate = true;
			}
		}
	}
	return isValidMove;
};
