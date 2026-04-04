#pragma once
#include "gameObject.h"

struct Play {
	float positionEvaluation;
	float atackEvaluation;

	BrickData brick;

	Play() : positionEvaluation(0), atackEvaluation(0), brick(BrickData())
	{
	}
	Play(std::vector<float> nums, BrickData b) : positionEvaluation(nums[0]), atackEvaluation(nums[1]), brick(b)
	{
	}

	auto operator<=>(const Play& other) const
	{
		return positionEvaluation + atackEvaluation <=> other.positionEvaluation + other.atackEvaluation;
	}

	auto operator==(const Play& other) const
	{
		return positionEvaluation + atackEvaluation == other.positionEvaluation + other.atackEvaluation;
	}

	float getTotalEvaluation() const
	{
		return positionEvaluation + atackEvaluation;
	}
};

struct AIPlayer
{
	GridData gridData;
	Timer AIPLayIntervalTimer;
	std::vector<std::vector<Play>> plays;
	int8_t analysisDepth;
	int8_t playsConsideredPerAnalysis;
	int8_t adjustmentIndexPlayRotation = 0;

	AIPlayer(GridData& AIgridData, Timer timer, int8_t ad, int8_t pc) :
		gridData(AIgridData),
		AIPLayIntervalTimer(timer),
		playsConsideredPerAnalysis(pc),
		analysisDepth(ad)
	{

	}
};

//std::vector<float> evaluatePosition(GridData& gridData, int atack, size_t thisGridIndex);
//std::vector<Play> AIDecideBestPlays(GridData& gridData, int gridIndex, int numBestPlays, Play lastPlay);
//void AIPlay(const SDLState& state, GameState& gs, Resources& res, GridData& gridData, int gridIndex, int numBestPlaysAnalised, int analysisDepth);

