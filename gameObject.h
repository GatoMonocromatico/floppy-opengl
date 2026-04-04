#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <vector>
#include <SDL3/SDL.h>
#include <array>
#include <unordered_set>
#include <iostream>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>

#include"Camera.h"
#include"texture.h"
#include "animation.h"
#include "shaderClass.h"
#include "Resources.h"

enum class ObjectType {
	brickUnit, level, grid
};

struct GameObject
{
	size_t specificDataLocation;
	ObjectType type;

	std::vector<Animation> animations;
	int32_t currentAnimation;
	size_t meshIDX;

	glm::mat4 model;

	void Draw(Resources& res, Shader& shader, Camera& camera);
	void Draw(Resources& res, Shader& shader, Camera& camera, int numIntaces);

	void translateModel(glm::vec3 translation);
	void rotateModel(glm::vec3 axis, float angle);
	void scaleModel(glm::vec3 scale);

	GameObject(ObjectType&& t, size_t loc, size_t mIdx, std::vector<Animation> anim = std::vector<Animation>{}, int32_t CA = -1) :
		type(t),
		specificDataLocation(loc),
		animations(anim),
		currentAnimation(CA),
		meshIDX(mIdx),
		model(glm::mat4(1))
	{
	}
	GameObject(ObjectType&& t, size_t loc, size_t mIdx, glm::mat4 m, std::vector<Animation> anim = std::vector<Animation>{}, int32_t CA = -1) :
		type(t),
		specificDataLocation(loc),
		animations(anim),
		currentAnimation(CA),
		meshIDX(mIdx),
		model(m)
	{
	}
};

enum class BrickState {
	idle, moving, solid
};

enum class Piece {
	T, I, S, Z, J, L, O, nullPiece
};

class BrickUnitData {
public:
	Piece shape;//brick has
	int8_t numeration;
	int8_t rotationState;//brick has
	std::array<glm::ivec2, 4> rotAdjustments;
	glm::ivec2 position;
	glm::ivec2 prevPosition;
	int16_t indexInCurrentBricks;//brick has
	uint32_t brickId;//brick has
	size_t idxCurrentUnits;

	explicit operator bool() const {
		return shape != Piece::nullPiece;
	}

	BrickUnitData() :
		shape(Piece::nullPiece),
		numeration(-1),
		rotationState(0),
		position(glm::ivec2(-1, -1)),
		prevPosition(glm::ivec2(-1, -1)),
		indexInCurrentBricks(-1),
		brickId(0),
		rotAdjustments(std::array<glm::ivec2, 4> {}),
		idxCurrentUnits(0)
	{
	}
	BrickUnitData(int x, int y, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id, size_t idxCurUnits) :
		shape(s),
		numeration(n),
		rotationState(0),
		position(glm::ivec2(x, y)),
		prevPosition(glm::ivec2(x, y)),
		indexInCurrentBricks(index),
		brickId(id),
		rotAdjustments(ra),
		idxCurrentUnits(idxCurUnits)
	{
	}
	BrickUnitData(glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id, size_t idxCurUnits) :
		shape(s),
		numeration(n),
		rotationState(0),
		position(pos),
		prevPosition(pos),
		indexInCurrentBricks(index),
		brickId(id),
		rotAdjustments(ra),
		idxCurrentUnits(idxCurUnits)
	{
	}
};



class LevelData {};

struct RowStatistics
{
	int numOfBricks;
	bool hasToUpdate = false;
	int8_t y;

	RowStatistics(int8_t pos) :
		numOfBricks(0),
		y(pos)
	{
	}

};
struct ColumnStatistics
{
	bool hasToUpdate = false;
	int8_t x;
	int8_t height = 0;
	int8_t uniqueConsecutiveGaps = 0;

	ColumnStatistics(int8_t pos) :
		x(pos)
	{
	}

};

struct GridCell
{
	int16_t brickIndex;
	int8_t unitNum;
	GridCell()
	{
		brickIndex = -1;
		unitNum = -1;
	}
	GridCell(int16_t num1, int8_t num2) : brickIndex(num1), unitNum(num2)
	{
	}
	explicit operator bool() const {
		return brickIndex != -1;
	}
};


class BrickData
{
public:
	BrickState state = BrickState::idle;
	Piece shape;

	int8_t rotationState;

	glm::vec2 velocity = glm::vec2(0);

	uint32_t brickId;
	int16_t indexInCurrentBricks;

	int8_t resets = 0;

	int8_t numDreprecatedUnits = 0;

	std::vector<GameObject> units;

	int8_t movingDirection = 0;

	bool wasntHandledSinceRotation = false;

	bool hasToUpdate = false;

	BrickData() :
		shape(Piece::nullPiece),
		indexInCurrentBricks(-1),
		brickId(0),
		rotationState(0)
	{
	}
	/*BrickData(Piece s) :
		shape(s),
		indexInCurrentBricks(0),
		brickId(0),
		rotationState(0)
	{
		units.resize(4);
	}*/

	BrickData(Piece s, unsigned int i, unsigned int id, int r = 0) :
		shape(s),
		indexInCurrentBricks(i),
		brickId(id),
		rotationState(r)
	{
	}
	/*BrickData(Piece s, unsigned int i, unsigned int id, int r) :
		shape(s),
		indexInCurrentBricks(i),
		brickId(id),
		rotationState(r)
	{
		units.resize(4);
	}*/
};

struct GridData
{
	int8_t gridColumns, gridRows;
	std::vector<std::vector<GridCell>> gridUnitsData;
	std::vector<RowStatistics> rowsStatus;
	std::vector<ColumnStatistics> columnsStatus;
	std::vector<std::array<int8_t, 3>> columnsAligningConsecutiveAtackGaps;

	int8_t rotationIndexNextBricks;

	std::vector<Piece>	nextBricks;
	std::vector<Piece> brickBuild;

	uint32_t idNextCreatedBrick;

	std::vector<BrickData> currentBricks;
	std::vector<BrickUnitData> currentUnits;

	std::vector<int16_t> bricksToUpdate;

	Piece reservedBrick;

	BrickData previewBrick;

	int8_t biggestYFallForPlayingBrick;

	GridData() :
		idNextCreatedBrick(1),
		gridColumns(10),
		gridRows(26),
		brickBuild(std::vector<Piece>{ Piece::T, Piece::S, Piece::Z, Piece::J, Piece::L, Piece::I, Piece::O }),
		reservedBrick(Piece::nullPiece),
		rotationIndexNextBricks(0),
		previewBrick(BrickData()),
		biggestYFallForPlayingBrick(27)
	{
		for (size_t i = 0; i < gridRows; i++)
		{
			rowsStatus.push_back(RowStatistics(i));
		}
		for (size_t i = 0; i < gridColumns; i++)
		{
			columnsStatus.push_back(ColumnStatistics(i));
		}

		gridUnitsData = std::vector<std::vector<GridCell>>(gridRows, std::vector<GridCell>(gridColumns, GridCell()));
	}
};
