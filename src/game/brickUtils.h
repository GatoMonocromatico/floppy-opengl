#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include<array>
#include<iostream>
#include "util/StdAliases.h"
#include "game/gameObject.h"

std::array<glm::ivec2, 4> createStartingPos(Piece s);
stdArrMat<glm::ivec2, 4, 4> createRotationalAdjustments(Piece s);
// Pool allocation. Handles returned here are stable for the lifetime of the entry:
// nothing is ever moved or renumbered, freed slots are recycled through the free
// lists instead. See docs/Entity Handles.md.
size_t createUnit(GridData& gridData, glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id);
size_t createUnit(GridData& gridData, int x, int y, std::array<glm::ivec2, 4> ra, Piece s, size_t index, uint32_t id);
size_t createUnit(GridData& gridData);
void freeUnit(GridData& gridData, size_t handle);

size_t createBrick(GridData& gridData, Piece shape, uint32_t id);
void freeBrick(GridData& gridData, size_t handle);

int getbiggestYFallForBrick(BrickData& brick, GridData& gridData);
bool isUnitPositionValid(GameObject& u, int x, int y, GridData& gridData);
bool updateBrickPositionTranslational(BrickData& brick, GridData& gridData, glm::ivec2 deltaPos, bool changeData = false);
bool updateBrickPositionRotation(BrickData& brick, GridData& gridData, int CWRotations, bool changeData = false);
