#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include<array>
#include<iostream>
#include "gameObject.h"

std::array<glm::ivec2, 4> createStartingPos(Piece s);
std::array<std::array<glm::ivec2, 4>, 4> createRotationalAdjustments(Piece s);
size_t createUnit(GridData& gridData, glm::ivec2 pos, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id);
size_t createUnit(GridData& gridData, int x, int y, std::array<glm::ivec2, 4> ra, Piece s, int n, size_t index, uint32_t id);
size_t createUnit(GridData& gridData);

int getbiggestYFallForBrick(BrickData& brick, GridData& gridData);
bool isUnitPositionValid(GameObject& u, int x, int y, GridData& gridData);
bool updateBrickPositionTranslational(BrickData& brick, GridData& gridData, glm::ivec2 deltaPos, bool changeData = false);
bool updateBrickPositionRotation(BrickData& brick, GridData& gridData, int CWRotations, bool changeData = false);
