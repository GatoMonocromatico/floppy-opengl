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

#include "util/StdAliases.h"
#include "render/camera.h"
#include "render/texture.h"
#include "render/animation.h"
#include "render/shaderClass.h"
#include "game/Resources.h"

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
	int8_t rotationState;//brick has
	std::array<glm::ivec2, 4> rotAdjustments;
	glm::ivec2 position;
	glm::ivec2 prevPosition;
	int16_t indexInCurrentBricks;//brick has
	uint32_t brickId;//brick has

	explicit operator bool() const {
		return shape != Piece::nullPiece;
	}

	BrickUnitData() :
		shape(Piece::nullPiece),
		rotationState(0),
		position(glm::ivec2(-1, -1)),
		prevPosition(glm::ivec2(-1, -1)),
		indexInCurrentBricks(-1),
		brickId(0),
		rotAdjustments(std::array<glm::ivec2, 4> {})
	{
	}
	BrickUnitData(glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id) :
		shape(s),
		rotationState(0),
		position(pos),
		prevPosition(pos),
		indexInCurrentBricks(static_cast<int16_t>(index)),
		brickId(id),
		rotAdjustments(ra)
	{
	}
	BrickUnitData(int x, int y, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id) :
		BrickUnitData(glm::ivec2(x, y), ra, s, index, id)
	{
	}
};



class LevelData {};

// Per-row occupancy count. This is the ONLY maintained statistic: cleanFullLines
// needs it to detect full rows in O(1). Board features used by the AI (heights,
// holes, bumpiness, wells) are computed inside AISim on simulated boards instead --
// maintaining them here would be a parallel structure that cannot be reused for the
// hypothetical boards the search actually evaluates. See docs/AI.md.
struct RowStatistics
{
	int numOfBricks;
	int8_t y;

	RowStatistics(int8_t pos) :
		numOfBricks(0),
		y(pos)
	{
	}
};

// A cell names the unit occupying it by its stable handle into
// GridData::currentUnits. The previous (brickIndex, unitNum) pair was two indices
// into vectors that both got compacted, so cells went stale on every line clear.
struct GridCell
{
	int32_t unitHandle;

	GridCell() : unitHandle(-1)
	{
	}
	explicit GridCell(int32_t handle) : unitHandle(handle)
	{
	}
	explicit operator bool() const {
		return unitHandle != -1;
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
	int gridColumns, gridRows;
	stdMat<GridCell> gridUnitsData;
	std::vector<RowStatistics> rowsStatus;

	int rotationIndexNextBricks;

	std::vector<Piece>	nextBricks;
	std::vector<Piece> brickBuild;

	uint32_t idNextCreatedBrick;

	// Pools. Entries are NEVER moved or renumbered: a handle stays valid for the
	// whole lifetime of what it names. Freed slots go on the matching free list and
	// get reused by the next allocation, which is what keeps the pools bounded.
	// Compacting these instead is what corrupted the heap -- see docs/Entity Handles.md.
	std::vector<BrickData> currentBricks;
	std::vector<BrickUnitData> currentUnits;
	std::vector<size_t> freeBricks;
	std::vector<size_t> freeUnits;

	// The one piece of ordering the game actually needs. Previously implicit as
	// currentBricks.back(), an invariant every call site had to remember to keep.
	int32_t playingBrickHandle = -1;

	std::vector<int16_t> bricksToUpdate;

	Piece reservedBrick;

	BrickData previewBrick;

	// Set true whenever the playing brick's position/rotation actually change
	// (successful move, rotation, or hard drop) or a new playing brick is
	// created. Consumed (and cleared) by the update pipeline in main.cpp to
	// decide whether previewBrick needs to be resynced -- replaces what used
	// to be inferred locally from updatePlayingBrick's return value.
	bool previewHasToUpdate = false;

	int biggestYFallForPlayingBrick = 27;

	BrickData& playingBrick() { return currentBricks[playingBrickHandle]; }
	const BrickData& playingBrick() const { return currentBricks[playingBrickHandle]; }

	GridData() :
		idNextCreatedBrick(1),
		gridColumns(10),
		gridRows(26),
		brickBuild(std::vector<Piece>{ Piece::T, Piece::S, Piece::Z, Piece::J, Piece::L, Piece::I, Piece::O }),
		reservedBrick(Piece::nullPiece),
		rotationIndexNextBricks(0),
		previewBrick(BrickData())
	{
		for (size_t i = 0; i < gridRows; i++)
		{
			rowsStatus.push_back(RowStatistics(i));
		}

		gridUnitsData = stdMat<GridCell>(gridRows, std::vector<GridCell>(gridColumns, GridCell()));
	}
};
