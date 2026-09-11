#include "game/gameCore.h"
#include "util/DebugLog.h"

void createPlayingBrick(GameState& gs, Resources& res, size_t thisGridIndex, Piece shape)
{
	DBG_IF(flux::verbose, "createPlayingBrick gridIndex", thisGridIndex);
	GridData& gridData = gs.grids[thisGridIndex];
	uint32_t& idNextCreatedBrick = gridData.idNextCreatedBrick;
	// Handle, not a reference: createUnit() below can grow currentUnits, which would
	// invalidate anything held across it.
	const size_t previewLoc = gridData.previewBrick.units[0].specificDataLocation;

	if (thisGridIndex == 0)
	{
		gs.fallLockInTimer.reset();
		gs.coutingLockIn = false;
	}

	if (shape == Piece::nullPiece)
	{
		std::uniform_int_distribution<int> dist(0, 6);
		shape = static_cast<Piece>(dist(gs.rng));

		DBG_IF(flux::verbose, "criou random");
	}

	const size_t brickHandle = createBrick(gridData, shape, idNextCreatedBrick);

	std::array<glm::ivec2, 4> pos = createStartingPos(shape);
	stdArrMat<glm::ivec2, 4, 4> RA = createRotationalAdjustments(shape);

	for (size_t i = 0; i < 4; i++)
	{
		size_t unitHandle = createUnit(gridData, pos[i], RA[i], shape, brickHandle, idNextCreatedBrick);
		gridData.currentBricks[brickHandle].units.push_back(GameObject(ObjectType::brickUnit, unitHandle, 2));
	}

	gridData.currentUnits[previewLoc].brickId = idNextCreatedBrick;
	gridData.currentUnits[previewLoc].position.x = -1;

	gridData.playingBrickHandle = static_cast<int32_t>(brickHandle);

	gridData.bricksToUpdate.push_back(static_cast<int16_t>(brickHandle));
	// Set on the pooled brick, not on a local copy. The old code flagged a stack
	// temporary after push_back, so the element in the vector never got the flag.
	gridData.currentBricks[brickHandle].hasToUpdate = true;
	gridData.previewHasToUpdate = true;
	gridData.biggestYFallForPlayingBrick = getbiggestYFallForBrick(gridData.currentBricks[brickHandle], gridData);

	DBGGRID_IF(flux::verbose && (thisGridIndex == 0), gridData);
	DBG_IF(flux::verbose && (thisGridIndex == 0), "createPlayingBrick gridData.fall", gridData.biggestYFallForPlayingBrick);

	idNextCreatedBrick += 1;
}

std::array<bool, 3> updatePlayingBrick(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, float deltaTime)
{
	std::array<bool, 3> playingBrickMoved{false, false, false};

	BrickData& brick = gridData.playingBrick();

	stdMat<GridCell>& grid = gs.grids[thisGridIndex].gridUnitsData;

	Timer& gravityTimer = thisGridIndex == 0 ? gs.gravityTimer : gs.gameGravityTimer;
	// that wont work for more than 2 grids

	gravityTimer.step(deltaTime);
	//handle gravity
	if (gravityTimer.isTimedOut())
	{
		gravityTimer.reset();
		DBG_IF(flux::verbose, "TRYING GRAVITY FALL");
		if (updateBrickPositionTranslational(brick, gridData, glm::ivec2(0, 1), true))
		{
			playingBrickMoved[1] = true;
		}
	}

	if (thisGridIndex == 0)
	{
		gs.preventAcidentalHardDropTimer.step(deltaTime);

		// continous piece handling
		if (state.keys[gs.configuratedKeys.instantSoftDrop])
		{
			if (updateBrickPositionTranslational(gs.grids[0].playingBrick(), gridData, glm::ivec2(0, gs.grids[0].biggestYFallForPlayingBrick), true))
			{
				DBG_IF(flux::verbose, "TRYING INSTANT SOFT DROP");
				playingBrickMoved[1] = true;
			}

			gs.grids[0].biggestYFallForPlayingBrick = 0;
		}

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
				DBG_IF(flux::verbose, "TRYING MOVE HORIZONTALLY");
				if (updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
				{
					brick.state = BrickState::moving;
					brick.wasntHandledSinceRotation = false;
					playingBrickMoved[0] = true;
					gridData.previewHasToUpdate = true;
				}

				gs.startMovTimer.step(deltaTime);
				break;
			}
			case BrickState::moving:
			{
				if (currentDirection != brick.movingDirection)
				{
					DBG_IF(flux::verbose, "TRYING MOVE HORIZONTALLY");
					if (!updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
					{
						brick.state = BrickState::idle;
						playingBrickMoved[0] = true;
					}
					else
					{
						gridData.previewHasToUpdate = true;

						if (brick.wasntHandledSinceRotation)
						{
							brick.wasntHandledSinceRotation = false;
						}
					}

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
						DBG_IF(flux::verbose, "TRYING MOVE HORIZONTALLY");
						if (!updateBrickPositionTranslational(brick, gridData, glm::ivec2(currentDirection, 0), true))
						{
							brick.state = BrickState::idle;
						}
						else
						{
							playingBrickMoved[0] = true;
							gridData.previewHasToUpdate = true;

							if (brick.wasntHandledSinceRotation)
							{
								brick.wasntHandledSinceRotation = false;
							}
						}
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
				DBG_IF(flux::verbose, "TRYING CW ROTATION");
				if (updateBrickPositionRotation(brick, gridData, 1, true))
				{
					playingBrickMoved[2] = true;
					gridData.previewHasToUpdate = true;
				}

				gs.flipBrickCWpressed = true;

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
				DBG_IF(flux::verbose, "TRYING CCW ROTATION");
				if (updateBrickPositionRotation(brick, gridData, 3, true))
				{
					playingBrickMoved[2] = true;
					gridData.previewHasToUpdate = true;
				}
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
				DBG_IF(flux::verbose, "TRYING 180 ROTATION");
				if (updateBrickPositionRotation(brick, gridData, 2, true))
				{
					playingBrickMoved[2] = true;
					gridData.previewHasToUpdate = true;
				}
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
			AIUpdate(gs, res, thisGridIndex, deltaTime);
		}
	}

	// Re-fetch rather than reusing `brick`, for two separate reasons.
	//
	// Lifetime: AIUpdate above can lock a piece, which spawns the next one and
	// push_back()s into currentBricks. That reallocates, and `brick` -- bound at the
	// top of this function -- dangles. ASan reports it as a heap-use-after-free here.
	// This is the pattern docs/Memory Safety.md describes: never hold a BrickData&
	// across a call that can allocate.
	//
	// Correctness: even without a reallocation, locking re-points playingBrickHandle
	// at the NEW piece, so `brick` names the piece that just landed. The fall
	// distance would describe the wrong brick for a frame.
	gridData.biggestYFallForPlayingBrick = getbiggestYFallForBrick(gridData.playingBrick(), gridData);
	DBG_IF(flux::verbose, "gridData.fall", gridData.biggestYFallForPlayingBrick);

	return playingBrickMoved;
}

AtackInfo playingPieceDropped(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks)
{
	DBG_IF(flux::verbose, "INICIANDO playingPieceDropped");

	std::vector<Piece>& nextBricks = gridData.nextBricks;
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	BrickData& playingBrick = gridData.playingBrick();

	playingBrick.state = BrickState::solid;

	for (GameObject& u : playingBrick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
		grid[uData.prevPosition.y][uData.prevPosition.x] = GridCell();
	}

	for (GameObject& u : playingBrick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
		grid[uData.position.y][uData.position.x] = GridCell(static_cast<int32_t>(u.specificDataLocation));

		// The one maintained statistic: cleanFullLines needs it to spot full rows
		// without rescanning the board.
		gridData.rowsStatus[uData.position.y].numOfBricks += 1;

		uData.prevPosition = uData.position;
	}

	DBG_IF(flux::verbose, "INICIANDO cleanFullLines");
	AtackInfo atack = cleanFullLines(gs, res, gridData, thisGridIndex);

	if (eraseBricks)
	{
		DBG_IF(flux::verbose, "eraseBricks");
		createPlayingBrick(gs, res, thisGridIndex, nextBricks[gridData.rotationIndexNextBricks]);
		stepNextBricks(gs, res, gridData, thisGridIndex);
	}
	DBG_IF(flux::verbose, "INICIANDO updateGrid");
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

AtackInfo cleanFullLines(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex)
{
	stdMat<GridCell>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;

	std::unordered_set<int> fullLines;

	AtackInfo atack;

	// checks for full lines, bottom row upward
	for (int i = 0; i < gridData.gridRows; i++)
	{
		int readRow = gridData.gridRows - 1 - i;

		if (gridData.rowsStatus[readRow].numOfBricks == 0) break;

		if (gridData.rowsStatus[readRow].numOfBricks == gridData.gridColumns)
		{
			fullLines.insert(readRow);
			atack.rowsEliminatedCords.push_back(static_cast<int8_t>(readRow));
		}
	}

	if (fullLines.empty()) return atack;

	// rowsEliminatedCords is filled bottom-up, so front() is the bottommost row
	// (largest y) and back() the topmost.
	atack.lowestRowEliminated = atack.rowsEliminatedCords.front();
	atack.highestRowEliminated = atack.rowsEliminatedCords.back();

	// int, not size_t: writeRow is decremented past 0 on a full board, and an
	// unsigned wrap here indexed rowsStatus far out of bounds.
	int writeRow = atack.lowestRowEliminated;
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
						BrickUnitData& uData = gridData.currentUnits[cell.unitHandle];

						uData.position.y = writeRow;

						BrickData& brick = currentBricks[uData.indexInCurrentBricks];

						if (!brick.hasToUpdate)
						{
							gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
							brick.hasToUpdate = true;
						}
					}
				}

				gridData.rowsStatus[writeRow].numOfBricks = gridData.rowsStatus[readRow].numOfBricks;
			}
			writeRow--;
		}
		else
		{
			atack.atack += 1;
			atack.unitsEliminated.push_back(std::vector<ClearedUnit>{});

			for (GridCell& cell : grid[readRow])
			{
				if (!cell) continue;

				BrickUnitData& uData = gridData.currentUnits[cell.unitHandle];

				atack.unitsEliminated.back().push_back(ClearedUnit{ uData.position, uData.shape });

				BrickData& brick = currentBricks[uData.indexInCurrentBricks];

				brick.numDreprecatedUnits += 1;
				if (!brick.hasToUpdate)
				{
					gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
					brick.hasToUpdate = true;
				}

				// Only marked dead here. The handle is released in updateGrid, once
				// the brick's unit list has been rescanned and the grid rebuilt.
				uData.shape = Piece::nullPiece;
			}
		}
	}

	// Everything from row 0 down to writeRow is empty now: rows below it were filled
	// by the shift, and the scan stopped at the first already-empty row above the
	// stack. Replaces a `writeRow - i` loop that could index negatively.
	for (int r = 0; r <= writeRow; ++r)
	{
		gridData.rowsStatus[r].numOfBricks = 0;
	}

	return atack;
}


void updateGrid(GameState& gs, Resources& res, GridData& gridData, size_t thisGridIndex, bool eraseBricks)
{	
	if (gridData.bricksToUpdate.empty()) return;

	stdMat<GridCell>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;

	DBG_IF(flux::verbose, "LIMPANDO GRID");
	// cleans grid
	for (int brickToUpdateIndex : gridData.bricksToUpdate)
	{
		for (GameObject& u : currentBricks[brickToUpdateIndex].units)
		{
			BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

			grid[uData.prevPosition.y][uData.prevPosition.x] = GridCell();
		}
	}

	// re-adds bricks
	for (int brickToUpdateIndex : gridData.bricksToUpdate)
	{
		DBG_IF(flux::verbose, "RE-ADDING BRICK", brickToUpdateIndex);
		BrickData& brickToUpdate = currentBricks[brickToUpdateIndex];

		for (GameObject& u : brickToUpdate.units)
		{
			BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

			if (uData)
			{
				grid[uData.position.y][uData.position.x] = GridCell(static_cast<int32_t>(u.specificDataLocation));

				uData.prevPosition = uData.position;
			}
		}

		if (eraseBricks)
		{
			// Drop dead units from the brick and release their pool slots. Nothing is
			// renumbered: survivors keep the handles they already had, so every
			// GridCell naming them stays valid. This replaces the old compaction,
			// which rewrote handles with an accumulator that counted kept units where
			// it needed removed ones, and wrote past the end of currentUnits.
			size_t writeIdx = 0;
			for (size_t readIdx = 0; readIdx < brickToUpdate.units.size(); ++readIdx)
			{
				GameObject& u = brickToUpdate.units[readIdx];

				if (gridData.currentUnits[u.specificDataLocation])
				{
					if (writeIdx != readIdx)
					{
						brickToUpdate.units[writeIdx] = std::move(u);
					}
					writeIdx++;
				}
				else
				{
					freeUnit(gridData, u.specificDataLocation);
				}
			}

			brickToUpdate.units.erase(brickToUpdate.units.begin() + writeIdx, brickToUpdate.units.end());
		}

		// A brick with no units left is released too. The shape guard makes a
		// duplicate entry in bricksToUpdate harmless -- freeing a slot twice would
		// hand the same handle out to two different bricks.
		if (eraseBricks && brickToUpdate.units.empty() && brickToUpdate.shape != Piece::nullPiece)
		{
			DBG_IF(flux::verbose, "FREEING BRICK", brickToUpdateIndex);
			freeBrick(gridData, brickToUpdateIndex);
		}
		else
		{
			brickToUpdate.hasToUpdate = false;
		}
	}

	gridData.bricksToUpdate.clear();
}
