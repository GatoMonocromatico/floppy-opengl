/*void AIPlay(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, int gridIndex, int numBestPlaysAnalised, int analysisDepth)
{
	std::vector<Piece>& nextBricks = gridData.nextBricks;

	std::vector<std::vector<GameObject>>& grid = gridData.gridUnitsData;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;
	BrickData& playingBrick = currentBricks[currentBricks.size() - 1];

	std::vector<float> finalEvaluations;

	std::vector<EvaluationInfo> firstPlaysConsidered = AIDecideBestPlays(state, gs, res, grid, currentBricks, gridIndex, numBestPlaysAnalised);


	std::vector<EvaluationInfo> playsConsideredNow;
	std::vector<EvaluationInfo> playsConsideredNextIteration;

	for (int i = 0; i < numBestPlaysAnalised; i++)
	{
		std::vector<std::vector<GameObject>> testGrid = grid;
		std::vector<BrickData> testCurrentBricks = currentBricks;
		std::vector<Piece> testNextBricks = nextBricks;

		// plays the brick
		testCurrentBricks[testCurrentBricks.size() - 1] = firstPlaysConsidered[i].brick;
		testCurrentBricks[testCurrentBricks.size() - 1].state = BrickState::solid;

		// creates new brick
		createPlayingBrick(gs, res, testCurrentBricks, gridIndex, testNextBricks[0]);
		gs.grids[gridIndex].idNextCreatedBrick -= 1;

		// updates position
		updateGrid(gs, res, testGrid, testCurrentBricks, gridIndex);

		playsConsideredNow = AIDecideBestPlays(state, gs, res, testGrid, testCurrentBricks, gridIndex, numBestPlaysAnalised, firstPlaysConsidered[i]);

		stepNextBricks(gs, res, testNextBricks, gridIndex);
		cleanFullLines(gs, testGrid, testCurrentBricks);


#ifdef DEBUG
		std::cout << "loop 1 -> " << i << "\n";
#endif // DEBUG

		for (int j = 0; j < analysisDepth; j++)
		{

#ifdef DEBUG
			std::cout << "loop 2 -> " << j << "\n";
#endif // DEBUG

			for (int k = 0; k < playsConsideredNow.size(); k++)
			{


#ifdef DEBUG
				std::cout << "loop 3 -> " << k << "\n";
				std::cout << "size playsConsideredNow -> " << playsConsideredNow.size() << "\n";
				std::cout << "piece -> " << static_cast<int>(testNextBricks[0]) << "\n\n";
#endif // DEBUG
				testCurrentBricks[testCurrentBricks.size() - 1] = playsConsideredNow[k].brick;
				testCurrentBricks[testCurrentBricks.size() - 1].state = BrickState::solid;

				// creates new brick
				createPlayingBrick(gs, res, testCurrentBricks, gridIndex, testNextBricks[0]);
				gs.grids[gridIndex].idNextCreatedBrick -= 1;

				// updates position
				updateGrid(gs, res, testGrid, testCurrentBricks, gridIndex);

				for (EvaluationInfo b : AIDecideBestPlays(state, gs, res, testGrid, testCurrentBricks, gridIndex, numBestPlaysAnalised))
				{
					playsConsideredNextIteration.push_back(b);

#ifdef DEBUG
					std::cout << "(interno)loop 3 -> " << k << "rodou" << "\n";
#endif // DEBUG

				}

			}

			stepNextBricks(gs, res, testNextBricks, gridIndex);
			cleanFullLines(gs, testGrid, testCurrentBricks);


			playsConsideredNextIteration.resize(0);
		}


#ifdef DEBUG
		std::cout << "1" << "\n";
#endif // DEBUG

		std::vector<float> thisEvaluations;

		for (EvaluationInfo eval : playsConsideredNow)
		{
			thisEvaluations.push_back(eval.getTotalEvaluation());
		}


#ifdef DEBUG
		std::cout << "2" << "\n";
#endif // DEBUG

		auto maxItThisEvaluations = std::max_element(thisEvaluations.begin(), thisEvaluations.end());


#ifdef DEBUG
		std::cout << "3" << "\n";
#endif // DEBUG

		finalEvaluations.push_back(*maxItThisEvaluations);


#ifdef DEBUG
		std::cout << "4" << "\n";
#endif // DEBUG
	}

	auto maxIt = std::max_element(finalEvaluations.begin(), finalEvaluations.end());

	playingBrick = firstPlaysConsidered[maxIt - finalEvaluations.begin()].brick;


#ifdef DEBUG
	std::cout << finalEvaluations[maxIt - finalEvaluations.begin()];
#endif // DEBUG

	playingPieceDropped(gs, res, grid, currentBricks, nextBricks, gridIndex);
}*/
/*
std::vector<float> evaluatePosition(GameState& gs, Resources& res, GridData& gridData, int atack, size_t thisGridIndex)
{

	auto towerAvarageHeightEval = [&](float h)
		{
			return (-(std::pow(h, 3) * 591.0f) / 5780.0f) + ((std::pow(h, 2) * 2231.0f) / 5780.0f) + ((19444.0f * h) / 1445.0f) + 20.0f;
		};
	auto towerHeightStandardDeviationEval = [&](float x)
		{
			return -std::pow(1 + x, 6) + 101;
		};
	//auto numSemiFilledLinesEval = [&](float x)
	//	{
	//		return 100.0f - 1.0f / std::pow(1.5, x - 11.357);
	//	};

	auto linesConfigurationEval = [&](int badX, int goodX)
		{
			return (100.0f - 1.0f / std::pow(1.5, goodX / 1.5f - 11.357)) - (100.0f - 1.0f / std::pow(1.5, (badX - 1) * 1.2f - 11.357));
		};
	auto atackEval = [&](float x)
		{
			return static_cast<float>(x > 1 ? std::pow(75, x / 3.5f) + 25 : 0);
		};
	auto numLinesWithCavitysEval = [&](float x)
		{

			return x < 3 ? 0 : std::pow(75, x / 2.0f) + 50;
		};
	auto alignedAtackLinesEval = [&](int x)
		{
			return  45.0f * std::log10(x + 1);
		};
	float eval = 0;
	//eval = 0;
	//	towerAvarageHeightEval(towerAvarageHeight) * 1
	//	+ towerHeightStandardDeviationEval(towerHeightStandardDeviation) * 10
	//	+ linesConfigurationEval(numBadLines, numGoodLines) * 1
	//	- numLinesWithCavitysEval(numLinesWithCavitys) * 2
	//	+ alignedAtackLinesEval(alignedAtackLines) * 1
	//	;

	return std::vector<float>{eval, atackEval(atack) * 2};
};

std::vector<Play> AIDecideBestPlays(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, int gridIndex, int numBestPlays, Play lastPlay)
{
	std::vector<Play> plays;
	std::vector<BrickData>& currentBricks = gridData.currentBricks;
	std::vector<std::vector<GridCell>>& grid = gridData.gridUnitsData;
	BrickData& playingBrick = currentBricks.back();

	std::vector<glm::ivec2> originalPositions;
	for (GameObject& u : playingBrick.units)
	{
		BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
		originalPositions.push_back(uData.position);
	}

	for (int i = 0; i < 4; i++)
	{
		if (updateBrickPositionRotation(gridData.currentBricks.back(), gridData, i))
		{
			while (updateBrickPositionTranslational(gridData.currentBricks.back(), gridData, glm::ivec2(-1, 0)))
			{
			}

			for (int c = 0; c < gridData.gridColumns; c++)
			{
				if (updateBrickPositionTranslational(gridData.currentBricks.back(), gridData, glm::ivec2(c, 0)))
				{
					while (updateBrickPositionTranslational(gridData.currentBricks.back(), gridData, glm::ivec2(0, 1)))
					{
					}

					for (int i = 0; i < 4; i++)
					{
						if (updateBrickPositionRotation(gridData.currentBricks.back(), gridData, i))
						{
							while (updateBrickPositionTranslational(gridData.currentBricks.back(), gridData, glm::ivec2(0, 1)))
							{
							}

							AtackInfo atackInfo = playingPieceDropped(gs, res, gridData, gridIndex, false);

							plays.push_back(Play(evaluatePosition(gs, res, gridData, atackInfo.atack, gridIndex), gridData.currentBricks.back()));

							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!
							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!
							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!


							//statistics update
							//statistics update
							//statistics update
							std::vector<int> rowsToUpdate;

							playingBrick.state = BrickState::idle;

							for (GameObject& u : playingBrick.units)
							{
								BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

								int& X = uData.position.x;
								int& Y = uData.position.y;

								RowStatistics& row = gridData.rowsStatus[Y];

								// adds unit to new row if it is not deprecated
								row.numOfBricks -= 1;

								X = originalPositions[uData.numeration].x;
								Y = originalPositions[uData.numeration].y;

								rowsToUpdate.push_back(Y);
							}

							std::sort(rowsToUpdate.begin(), rowsToUpdate.end());

							//for (std::array<int8_t, 3>& gapInfo : gridData.columnsAligningConsecutiveAtackGaps)
							//{
							//	if (rowsToUpdate.front())
							//}

							if (!playingBrick.hasToUpdate)
							{
								gridData.bricksToUpdate.push_back(playingBrick.indexInCurrentBricks);
								playingBrick.hasToUpdate = true;
							}
							//statistics update
							//statistics update
							//statistics update

							updateGrid(gs, res, gridData, gridIndex, false);

							if (atackInfo.rowsWereEliminated())
							{
								int eliminatedRowIndex = 0;
								for (int i = atackInfo.lowestRowEliminated; i > 0; i--)
								{
									// if row was deleted, restore the data their units especific data
									if (i == atackInfo.rowsEliminatedCords[eliminatedRowIndex])
									{
										for (GameObject& u : atackInfo.unitDataRowsEliminated[eliminatedRowIndex])
										{
											BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];
											BrickData& unitsBrick = gridData.currentBricks[uData.indexInCurrentBricks];

											unitsBrick.numDreprecatedUnits -= 1;

											uData.shape = unitsBrick.shape;
											if (!unitsBrick.hasToUpdate)
											{
												gridData.bricksToUpdate.push_back(unitsBrick.indexInCurrentBricks);
												unitsBrick.hasToUpdate = true;
											}
										}
										eliminatedRowIndex += 1;
									}
									// if all line data was restored, break, and update columns height
									// it updates the columns height here because the loop already scaned
									// the highest line possible for any column (little optimization)
									else if (gridData.rowsStatus[i].numOfBricks == 0)
									{
										//statistics update
										//statistics update
										//statistics update
										updateGrid(gs, res, gridData, gridIndex, false);

										for (int i = 0; i < gridData.gridColumns; i++)
										{
											int maxPossibleHeight = gridData.columnsStatus[i].height + atackInfo.rowsEliminatedCords.size();
											int j = maxPossibleHeight > i + 1 ? i + 1 : maxPossibleHeight;
											for (j; j < gridData.gridRows; j++)
											{
												if (grid[j][i])
												{
													gridData.columnsStatus[i].height = gridData.gridRows - j;
													break;
												}

											}
											if (j == gridData.gridRows - 1) gridData.columnsStatus[i].height = 0;
										}
										//statistics update
										//statistics update
										//statistics update
										break;
									}

									// restores units position y before line elimination
									for (GridCell& cell : gridData.gridUnitsData[i])
									{
										if (cell)
										{
											GameObject& u = gridData.currentBricks[cell.brickIndex].units[cell.unitNum];
											BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

											uData.position.y -= eliminatedRowIndex;

											BrickData& brick = gridData.currentBricks[uData.indexInCurrentBricks];
											if (!brick.hasToUpdate)
											{
												gridData.bricksToUpdate.push_back(brick.indexInCurrentBricks);
												brick.hasToUpdate = true;
											}
										}
									}
								}

							}

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

									int gapStartY = row.y;
									int gapEndY = row.y;
									int gapSize = 1;

									for (int increment = -1; increment <= 1; increment += 2)
									{

										int yPosBeingRead = row.y + increment;
										while (yPosBeingRead < gridData.gridRows && yPosBeingRead >= 0)
										{
											RowStatistics& readingRow = gridData.rowsStatus[yPosBeingRead];
											readingRow.hasToUpdate = false;

											if (readingRow.numOfBricks == gridData.gridColumns - 1)
											{
												if (!gridData.gridUnitsData[yPosBeingRead][gapXCord])
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
										gridData.columnsAligningConsecutiveAtackGaps.push_back(std::array<int8_t, 3>{static_cast<int8_t>(gapXCord), static_cast<int8_t>(gapStartY), static_cast<int8_t>(gapEndY)});
									}
								}

								row.hasToUpdate = false;
							}
							//statistics update
							//statistics update
							//statistics update
							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!
							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!
							//DESFAZER TUUUUUUUUUUDO!!!!!!!!!!!!!!!!!!!!!
						}
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	std::sort(plays.begin(), plays.end());

	std::vector<Play> choices(plays.end() - 2, plays.end());

	return choices;
}
*/