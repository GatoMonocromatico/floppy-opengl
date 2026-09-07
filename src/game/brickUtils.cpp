#include "game/brickUtils.h"

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
		std::cout << "INVALID PIECE DETECTED!!!!!!!!!!!\n";
		break;
	}
}

std::array<std::array<glm::ivec2, 4>, 4> createRotationalAdjustments(Piece s) {
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
		std::cout << "INVALID PIECE DETECTED!!!!!!!!!!!\n";
		break;
	}
}

size_t createUnit(GridData& gridData, glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id)
{
	gridData.currentUnits.push_back(BrickUnitData(pos, ra, s, n, index, id, gridData.currentUnits.size()));

	return gridData.currentUnits.size() - 1;
}
size_t createUnit(GridData& gridData, int x, int y, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id)
{
	gridData.currentUnits.push_back(BrickUnitData(x, y, ra, s, n, index, id, gridData.currentUnits.size()));

	return gridData.currentUnits.size() - 1;
}
size_t createUnit(GridData& gridData)
{
	gridData.currentUnits.push_back(BrickUnitData());

	return gridData.currentUnits.size() - 1;
}


int getbiggestYFallForBrick(BrickData& brick, GridData& gridData)
{
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
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

	return lowestDeltaY;
}


bool isUnitPositionValid(GameObject& u, int x, int y, GridData& gridData)
{
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
	BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

	bool validation = true;

	if (x < 0 || x > 9 || y < 0 || y > 25)
	{
		validation = false;
	}
	else if (grid[y][x] && grid[y][x].brickIndex != uData.indexInCurrentBricks)
	{
		validation = false;
		std::cout << "invalid position: already has a brick in position\n";

	}

	return validation;
};


bool updateBrickPositionTranslational(BrickData& brick, GridData& gridData, glm::ivec2 deltaPos, bool changeData)
{
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
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
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
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

	if (!isValidMove)
	{
		std::array<std::vector<glm::ivec2>, 3> testAdjustments;

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
			if (!brick.hasToUpdate)
			{
				gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
				brick.hasToUpdate = true;
			}
		}
	}
	return isValidMove;
};
