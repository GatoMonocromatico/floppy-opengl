#include "game/gameCore.h"

void createPlayingBrick(GameState& gs, Resources& res, std::vector<BrickData>& currentBricks, size_t thisGridIndex, Piece shape)
{
	uint32_t& idNextCreatedBrick = gs.grids[thisGridIndex].idNextCreatedBrick;
	GridData& gridData = gs.grids[thisGridIndex];
	GameObject& previewU = gridData.previewBrick.units[0];
	BrickUnitData& previewUData = gridData.currentUnits[previewU.specificDataLocation];

	gs.fallLockInTimer.reset();
	gs.coutingLockIn = false;

	if (shape == Piece::nullPiece)
	{
		std::uniform_int_distribution<int> dist(0, 6);

		int randomShape = dist(gs.rng);
		shape = static_cast<Piece>(randomShape);

#ifdef DEBUG
		std::cout << "criou random";
#endif // DEBUG
	}
	BrickData brick(shape, currentBricks.size(), idNextCreatedBrick);

	auto initializeUnits = [&](Piece s, const std::array<glm::ivec2, 4>  pos, const std::array<std::array<glm::ivec2, 4>, 4>& RA) {
		for (size_t i = 0; i < 4; i++)
		{
			brick.units.push_back(GameObject(ObjectType::brickUnit, createUnit(gridData, pos[i], RA[i], s, i, brick.indexInCurrentBricks, brick.brickId), 2));

			previewUData.brickId = brick.brickId;
		}
		};

	std::array<glm::ivec2, 4> pos = createStartingPos(shape);
	std::array<std::array<glm::ivec2, 4>, 4> RA = createRotationalAdjustments(shape);

	initializeUnits(shape, pos, RA);

	currentBricks.push_back(brick);

	gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
	brick.hasToUpdate = true;

	previewUData.position.x = -1;

	idNextCreatedBrick += 1;
}

void updateBricks(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, BrickData& brick, size_t thisGridIndex, float deltaTime)
{
	std::vector<std::vector<GridCell>>& grid = gs.grids[thisGridIndex].gridUnitsData;

	if (brick.state != BrickState::solid) {
		if (gs.coutingLockIn) {
			gs.fallLockInTimer.step(deltaTime);
		}

		Timer& gravityTimer = thisGridIndex == 0 ? gs.gravityTimer : gs.gameGravityTimer;
		// that wont work for more than 2 grids

		gravityTimer.step(deltaTime);
		//handle gravity
		if (gravityTimer.isTimedOut())
		{

			gravityTimer.reset();
			updateBrickPositionTranslational(brick, gridData, glm::ivec2(0, 1), true);
		}


		if (thisGridIndex == 0)
		{
			if (state.keys[gs.configuratedKeys.instantSoftDrop])
			{
				updateBrickPositionTranslational(gs.grids[0].currentBricks.back(), gridData, glm::ivec2(0, gs.grids[0].biggestYFallForPlayingBrick), true);
				gs.grids[0].biggestYFallForPlayingBrick = 0;
			}


			gs.preventAcidentalHardDropTimer.step(deltaTime);

			//handle piece handling
			int currentDirection = 0;

			if (state.keys[gs.configuratedKeys.moveRight])
			{
				currentDirection += 1;
			}
			if (state.keys[gs.configuratedKeys.moveLeft])
			{
				currentDirection -= 1;
			}
			if (currentDirection)
			{
				switch (brick.state)
				{
				case BrickState::idle:
				{
					brick.state = BrickState::moving;

					if (!updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
					{
						brick.state = BrickState::idle;
					}
					brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;

					gs.startMovTimer.step(deltaTime);
					break;
				}
				case BrickState::moving:
				{
					if (currentDirection != brick.movingDirection)
					{
						if (!updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
						{
							brick.state = BrickState::idle;
						}
						brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;

						gs.startMovTimer.reset();
						gs.repeatedMovTimer.fullReset();

					}

					gs.startMovTimer.step(deltaTime);

					if (gs.startMovTimer.isTimedOut())
					{
						gs.repeatedMovTimer.step(deltaTime);

						if (gs.repeatedMovTimer.isTimedOut())
						{
							gs.repeatedMovTimer.reset();
							if (!updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
							{
								brick.state = BrickState::idle;
							}
							brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;

						}

					}
					break;
				}
				}
			}
			else
			{
				brick.state = BrickState::idle;

				gs.startMovTimer.reset();


				gs.repeatedMovTimer.fullReset();
			}

			brick.movingDirection = currentDirection;

			if (state.keys[gs.configuratedKeys.rotateCW])
			{
				if (!gs.flipBrickCWpressed)
				{
					updateBrickPositionRotation(brick, gridData, 1, true);

					gs.flipBrickCWpressed = true;

					brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;
				}
			}
			else
			{
				gs.flipBrickCWpressed = false;
			}

			if (state.keys[gs.configuratedKeys.rotateCCW])
			{
				if (!gs.flipBrickCCWpressed)
				{
					updateBrickPositionRotation(brick, gridData, 3, true);
					brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;
				}
				gs.flipBrickCCWpressed = true;
			}
			else
			{
				gs.flipBrickCCWpressed = false;
			}
			if (state.keys[gs.configuratedKeys.rotate180])
			{
				if (!gs.flipBrick180pressed)
				{
					updateBrickPositionRotation(brick, gridData, 2, true);
					brick.wasntHandledSinceRotation = brick.state == BrickState::idle ? true : false;
				}
				gs.flipBrick180pressed = true;
			}
			else
			{
				gs.flipBrick180pressed = false;
			}
		}
		else
		{
			if (brick.state != BrickState::solid)
			{
				//		gs.AIPLayIntervalTimers[thisGridIndex - 1].step(deltaTime);
				//		if (gs.AIPLayIntervalTimers[thisGridIndex - 1].isTimedOut())
				//		{
				//			//AIPlay(state, gs, res, thisGridIndex, 1, 0);
				//			gs.AIPLayIntervalTimers[thisGridIndex - 1].reset();
				//		}
			}
		}

	}

}

AtackInfo playingPieceDropped(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks)
{
	// statistic purpose
	std::vector<int> rowsToUpdate;
	// statistic purpose

	std::vector<Piece>& nextBricks = gridData.nextBricks;

	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;

	BrickData& playingBrick = currentBricks.back();

	playingBrick.state = BrickState::solid;

	for (GameObject& u : playingBrick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
		grid[uData.prevPosition.y][uData.prevPosition.x] = GridCell();
	}

	for (GameObject& u : playingBrick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
		grid[uData.position.y][uData.position.x].brickIndex = uData.indexInCurrentBricks;
		grid[uData.position.y][uData.position.x].unitNum = uData.numeration;

		//statistics update
		//statistics update
		//statistics update
		int& X = uData.position.x;
		int& Y = uData.position.y;

		RowStatistics& row = gridData.rowsStatus[Y];

		// adds unit to new row if it is not deprecated
		row.numOfBricks += 1;

		// updates new column height
		if (gridData.gridRows - Y > gridData.columnsStatus[X].height)
		{
			gridData.columnsStatus[X].height = static_cast<int8_t>(gridData.gridRows - Y);
		}

		if (!row.hasToUpdate)
		{
			row.hasToUpdate = true;
			rowsToUpdate.push_back(Y);
		}
		//statistics update
		//statistics update
		//statistics update

		uData.prevPosition = uData.position;
	}

	//statistics update
	//statistics update
	//statistics update
	for (int& rowIdx : rowsToUpdate)
	{
		RowStatistics& row = gridData.rowsStatus[rowIdx];

		//checando por atack gaps
		if (row.numOfBricks == gridData.gridColumns - 1 && row.hasToUpdate)
		{
			int gapXCord = 0;
			for (gapXCord; gapXCord < gridData.gridColumns; gapXCord++)
			{
				if (gridData.gridUnitsData[rowIdx][gapXCord]) break;
			}

			/*if (gridData.columnsStatus[gapXCord].uniqueConsecutiveGaps > 0)
			{
				bool alreadyRegistered = false;
				for (std::array<int8_t, 3>& gapInfo : gridData.columnsAligningConsecutiveAtackGaps)
				{
					if (gapInfo[0] == gapXCord)
					{
						if (rowIdx >= gapInfo[1] && rowIdx <= gapInfo[2])
						{
							break;

						}
					}
				}
			}*/

			int gapStartY = row.y;
			int gapEndY = row.y;
			int gapSize = 1;

			for (int increment = -1; increment <= 1; increment += 2)
			{

				int yPosBeingRead = row.y + increment;
				while (yPosBeingRead < gridData.gridRows && yPosBeingRead >= 0)
				{
					RowStatistics& readingRow = gridData.rowsStatus[rowIdx];
					readingRow.hasToUpdate = false;

					if (readingRow.numOfBricks == gridData.gridColumns - 1)
					{
						if (!gridData.gridUnitsData[rowIdx][gapXCord])
						{
							gapSize += 1;

							if (increment == -1) gapStartY = yPosBeingRead;
							else                   gapEndY = yPosBeingRead;
						}
						else break;

					}
					else if (readingRow.numOfBricks < gridData.gridColumns - 1) break;

					yPosBeingRead += increment;
				}
			}

			if (gapSize > 1)
			{
				gridData.columnsStatus[gapXCord].uniqueConsecutiveGaps += 1;
				gridData.columnsAligningConsecutiveAtackGaps.push_back(std::array<int8_t, 3>{static_cast<int8_t>(gapXCord), static_cast<int8_t>(gapStartY), static_cast<int8_t>(gapEndY)});
			}
		}

		row.hasToUpdate = false;
	}
	//statistics update
	//statistics update
	//statistics update

	AtackInfo atack = cleanFullLines(gs, res, gridData, thisGridIndex);

	if (eraseBricks)
	{
		createPlayingBrick(gs, res, currentBricks, thisGridIndex, nextBricks[gridData.rotationIndexNextBricks]);
		stepNextBricks(gs, res, gridData, thisGridIndex);
	}
	updateGrid(gs, res, gridData, thisGridIndex, eraseBricks);

	return atack;
}


void stepNextBricks(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex)
{
	std::vector<Piece>& nextBricks = gridData.nextBricks;
	int adj = gridData.rotationIndexNextBricks;

	std::uniform_int_distribution<int> dist(0, gs.grids[thisGridIndex].brickBuild.size() - 1);

	nextBricks[adj] = gridData.brickBuild[dist(gs.rng)];

	gridData.rotationIndexNextBricks = static_cast<int8_t>((adj + 1) % static_cast<int>(gridData.nextBricks.size()));
}

void eraseBricksFromCurrentBricks(GridData& gridData, std::vector<int16_t>& indexesOfDeprecatedBricks)
{
	if (indexesOfDeprecatedBricks.empty()) return;

#ifdef DEBUG
	std::cout << "erasing bricks\n";
#endif // DEBUG

	std::vector<BrickData>& currentBricks = gridData.currentBricks;


	std::unordered_set<int16_t> toDelete(indexesOfDeprecatedBricks.begin(), indexesOfDeprecatedBricks.end());

	size_t writeIndex = 0;
	for (size_t readIndex = 0; readIndex < currentBricks.size(); ++readIndex)
	{
		// readed element not in toDelete set
		if (toDelete.find(readIndex) == toDelete.end())
		{
			if (writeIndex != readIndex)
			{
				currentBricks[writeIndex] = std::move(currentBricks[readIndex]);
				currentBricks[writeIndex].indexInCurrentBricks = writeIndex;

				for (GameObject& u : currentBricks[writeIndex].units)
				{
					BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
					uData.indexInCurrentBricks = writeIndex;

					gridData.gridUnitsData[uData.position.y][uData.position.x].brickIndex = writeIndex;
				}
			}

			writeIndex++;
		}
	}

	currentBricks.erase(currentBricks.begin() + writeIndex, currentBricks.end());

#ifdef DEBUG
	std::cout << "erasing completed!!!!\n";
#endif // DEBUG
}


AtackInfo cleanFullLines(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex)
{
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;

	std::unordered_set<size_t> fullLines;

	AtackInfo atack;

	// checks for full lines
	for (size_t i = 0; i < gridData.gridRows; i++)
	{
		size_t readRow = gridData.gridRows - 1 - i;

		if (gridData.rowsStatus[readRow].numOfBricks == 0) break;

		bool fullLineFound = gridData.rowsStatus[readRow].numOfBricks == gridData.gridColumns;

#ifdef DEBUG
		for (int pos : gridData.rowsStatus[i].positionsFilled)
		{
			std::cout << pos << " ";
		}
		std::cout << "\n";
		std::cout << i << " -> " << fullLineFound << "(" << std::accumulate(gridData.rowsStatus[i].positionsFilled.begin(), gridData.rowsStatus[i].positionsFilled.end(), 0) << " == " << gridData.gridColumns << ")" << "\n";

#endif // DEBUG

		if (fullLineFound)
		{
			fullLines.insert(readRow);
			atack.rowsEliminatedCords.push_back(static_cast<int8_t>(readRow));
		}
	}

	if (fullLines.empty()) return atack;

	atack.lowestRowEliminated = static_cast<int8_t>(atack.rowsEliminatedCords.front());
	atack.highestRowEliminated = static_cast<int8_t>(atack.rowsEliminatedCords.back());

	size_t writeRow = atack.lowestRowEliminated;
	for (int readRow = atack.lowestRowEliminated; readRow > -1; readRow--)
	{
		// readed row not in fullLines set
		if (fullLines.find(readRow) == fullLines.end())
		{
			if (gridData.rowsStatus[readRow].numOfBricks == 0) break;

			if (writeRow != readRow)
			{
				for (GridCell& cell : grid[readRow])
				{
					if (cell)
					{
						GameObject& u = currentBricks[cell.brickIndex].units[cell.unitNum];
						BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

						uData.position.y = writeRow;

						BrickData& brick = currentBricks[uData.indexInCurrentBricks];

						if (!brick.hasToUpdate)
						{
							gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
							brick.hasToUpdate = true;
						}
					}
				}
				// statistics update
				// statistics update
				// statistics update
				gridData.rowsStatus[writeRow].numOfBricks = std::move(gridData.rowsStatus[readRow].numOfBricks);
				// statistics update
				// statistics update
				// statistics update
			}
			writeRow--;
		}
		else
		{
			atack.atack += 1;
			atack.unitDataRowsEliminated.push_back(std::vector<GameObject>{});

			for (GridCell& cell : grid[readRow])
			{
				GameObject& u = currentBricks[cell.brickIndex].units[cell.unitNum];
				BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

				atack.unitDataRowsEliminated.back().push_back(u);

				//T, I, S, Z, J, L, O, nullPiece
				uData.shape = Piece::nullPiece;

				BrickData& brick = currentBricks[uData.indexInCurrentBricks];

				brick.numDreprecatedUnits += 1;
				if (!brick.hasToUpdate)
				{
					gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
					brick.hasToUpdate = true;
				}
			}
		}
	}

	// statistics update
	// statistics update
	// statistics update
	if (!fullLines.empty())
	{
		for (int i = 0; i < atack.rowsEliminatedCords.size(); ++i)
		{
			int idx = writeRow - i;
			gridData.rowsStatus[idx].numOfBricks = 0;
		}

		for (int i = 0; i < gridData.gridColumns; i++)
		{
			int height = gridData.columnsStatus[i].height;
			if (height > atack.highestRowEliminated)
			{
				gridData.columnsStatus[i].height -= static_cast<int8_t>(atack.rowsEliminatedCords.size());

			}
			else
			{
				if (atack.highestRowEliminated == gridData.gridRows - 1)
				{
					gridData.columnsStatus[i].height = 0;
					continue;
				}
				for (int j = atack.highestRowEliminated + 1; j < gridData.gridRows; j++)
				{
					if (grid[j][i])
					{
						gridData.columnsStatus[i].height = gridData.gridRows - j;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < gridData.columnsAligningConsecutiveAtackGaps.size(); i++)
	{
		std::array<int8_t, 3>& atackGap = gridData.columnsAligningConsecutiveAtackGaps[i];
		int8_t& column = atackGap[0];
		int8_t& startY = atackGap[1];
		int8_t& endY = atackGap[2];
		int8_t gapSize = endY - startY + 1;
		if (startY > atack.lowestRowEliminated)
		{
			continue;
		}
		else if (endY < atack.highestRowEliminated)
		{
			startY += atack.rowsEliminatedCords.size();
			endY += atack.rowsEliminatedCords.size();
		}
		else if (endY <= atack.lowestRowEliminated && startY >= atack.highestRowEliminated)
		{
			gridData.columnsStatus[column].uniqueConsecutiveGaps -= 1;
			gridData.columnsAligningConsecutiveAtackGaps.erase(gridData.columnsAligningConsecutiveAtackGaps.begin() + i);
		}
		else if (endY <= atack.lowestRowEliminated)
		{
			endY = atack.highestRowEliminated - 1 + atack.rowsEliminatedCords.size();
			startY += atack.rowsEliminatedCords.size();
		}
		else if (startY >= atack.highestRowEliminated)
		{
			startY = atack.lowestRowEliminated + 1;
		}
	}
	// statistics update
	// statistics update
	// statistics update


	return atack;
}


void updateGrid(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks)
{
	if (gridData.bricksToUpdate.empty()) return;


	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;

	std::vector<int16_t> indexesOfDeprecatedBricks;

	// cleans grid
	for (int brickToUpdateIndex : gridData.bricksToUpdate)
	{
		for (GameObject& u : currentBricks[brickToUpdateIndex].units)
		{
			BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

			grid[uData.prevPosition.y][uData.prevPosition.x] = GridCell();
		}
	}

	size_t acumulatedDeltaFromPrevBricks = 0;

	// re-adds bricks
	for (int brickToUpdateIndex : gridData.bricksToUpdate)
	{
		BrickData& brickToUpdate = currentBricks[brickToUpdateIndex];

		if (brickToUpdate.numDreprecatedUnits == 4)
		{
			indexesOfDeprecatedBricks.push_back(brickToUpdate.indexInCurrentBricks);
		}

		bool brickIsSolid = brickToUpdate.state == BrickState::solid;
		std::unordered_set<int> deprecatedUnitsIdx;


		for (int i = 0; i < brickToUpdate.units.size(); i++)
		{
			GameObject& u = brickToUpdate.units[i];
			BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

			if (uData)
			{
				grid[uData.position.y][uData.position.x].brickIndex = uData.indexInCurrentBricks;
				grid[uData.position.y][uData.position.x].unitNum = uData.numeration;

				uData.prevPosition.y = uData.position.y;
				uData.prevPosition.x = uData.position.x;
			}
			else
			{
				deprecatedUnitsIdx.insert(i);
			}
		}

		if (eraseBricks)
		{
			// removes deprecated units from brick
			size_t writeIdx = 0;
			for (size_t readIdx = 0; readIdx < brickToUpdate.units.size(); ++readIdx)
			{
				if (deprecatedUnitsIdx.find(readIdx) == deprecatedUnitsIdx.end())
				{
					if (writeIdx != readIdx)
					{
						GameObject& u = brickToUpdate.units[writeIdx];
						u = std::move(brickToUpdate.units[readIdx]);
						BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

						uData.numeration = writeIdx;

						grid[uData.position.y][uData.position.x].unitNum = writeIdx;

						u.specificDataLocation = u.specificDataLocation - (readIdx - writeIdx) - acumulatedDeltaFromPrevBricks;

						gridData.currentUnits[u.specificDataLocation] = std::move(uData);
					}

					writeIdx++;
				}
			}
			acumulatedDeltaFromPrevBricks += writeIdx;

			brickToUpdate.units.erase(brickToUpdate.units.begin() + writeIdx, brickToUpdate.units.end());
		}

		brickToUpdate.hasToUpdate = false;
	}

	if (eraseBricks) eraseBricksFromCurrentBricks(gridData, indexesOfDeprecatedBricks);

	gridData.bricksToUpdate.clear();
}