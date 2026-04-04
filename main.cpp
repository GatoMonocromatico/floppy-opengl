#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <format>
#include <numeric>
#include <unordered_set>

#include "gameObject.h"
#include "Timer.h"
#include <cmath>

#include"animation.h"
#include"Mesh.h"
#include"Point.h"
#include"Light.h"
#include "brickUtils.h"
#include"Resources.h"
#include "GameState.h"
#include "gameCore.h"


bool moveCamera = false;

SDLState initialize(int width, int height);
void cleanup(SDLState& state);
void drawGrid(SDLState& state, Resources& res, GameState& gs, GridData& gridData, float deltaTime, size_t gridIndex);
//void updateUnitInGridStatistics(GameState& gs, GridData& gridData, GameObject& u, size_t thisGridIndex);


int main(int argc, char* argv[])
{
	SDLState state = initialize(1600, 900);

	if (!state.successfullyInitialized)
	{
		cleanup(state);
		return 1;
	}

	float cameraSpeed = 2;

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// CREATE RESORCES DATA STRUCTURE AND INITIALIZE IT HERE (GROUP TEXTURES/ANIMATIONS)
	std::vector<Shader> shaders = { Shader("default.vert", "default.frag"), Shader("default.vert", "brickUnit.frag"), Shader("point.vert", "point.frag") };

	Resources res(shaders);
	
	res.load();

	//setup game data
	GameState gs(res);

	for (size_t i = 0; i < gs.numOfGrids; i++)
	{
		createPlayingBrick(gs, res, gs.grids[i].currentBricks, i, gs.grids[i].nextBricks[0]);
		stepNextBricks(gs, res, gs.grids[i], i);
	}

	int fps = 0;

	//starting game loop
	bool running = true;

	//time management
	uint64_t prevTime = SDL_GetTicks();
	while (running)
	{
		uint64_t nowTime = SDL_GetTicks();
		float deltaTime = (nowTime - prevTime) / 1000.0f;

		SDL_Event event{ 0 };

		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_EVENT_MOUSE_MOTION && moveCamera) {
				res.camera.yaw += event.motion.xrel * res.camera.sensitivity;
				res.camera.pitch -= event.motion.yrel * res.camera.sensitivity;

				if (res.camera.pitch > 84.0f) res.camera.pitch = 84.0f;
				if (res.camera.pitch < -84.0f) res.camera.pitch = -84.0f;
				//if (res.camera.yaw > -44.0f) res.camera.yaw = -44.0f;
				//if (res.camera.yaw < -134.0f) res.camera.yaw = -134.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(res.camera.yaw)) * cos(glm::radians(res.camera.pitch));
				direction.y = sin(glm::radians(res.camera.pitch));
				direction.z = sin(glm::radians(res.camera.yaw)) * cos(glm::radians(res.camera.pitch));
				res.camera.orientation = glm::normalize(direction);

			}

			switch (event.type)
			{
			case SDL_EVENT_QUIT:
			{
				running = false;
				break;
			}
			case SDL_EVENT_WINDOW_RESIZED: {
				state.width = event.window.data1;
				state.height = event.window.data2;
				break;
			}
			case SDL_EVENT_KEY_DOWN:

				if (event.key.scancode == SDL_SCANCODE_O)
				{
					moveCamera = !moveCamera;
					SDL_SetWindowRelativeMouseMode(state.window, moveCamera);
				}
				if (event.key.scancode == SDL_SCANCODE_P)
				{
					res.camera.yaw = -90.0f;
					res.camera.pitch = 0.0f;
					res.camera.orientation = glm::vec3(0.0f, 0.0f, -1.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_DOWN)
				{
					res.camera.position += glm::vec3(0.0f, 0.0f, 1.00f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_LCTRL)
				{
					res.camera.position -= glm::vec3(0.0f, 1.00f, 0.0f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_LSHIFT)
				{
					res.camera.position += glm::vec3(0.0f, 1.00f, 0.0f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_UP)
				{
					res.camera.position -= glm::vec3(0.0f, 0.0f, 1.00f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_RIGHT)
				{
					res.camera.position += glm::vec3(1.00f, 0.0f, 0.0f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (event.key.scancode == SDL_SCANCODE_LEFT)
				{
					res.camera.position -= glm::vec3(1.00f, 0.0f, 0.0f) * cameraSpeed * deltaTime;
					res.scenarioLights[1].position = glm::vec4(res.camera.position, 0.0f);
				}
				if (gs.coutingLockIn)
				{
					for (SDL_Scancode keyCode : gs.configuratedKeys.handlingKeys)
					{
						if (event.key.scancode == keyCode)
						{
							std::vector<BrickData>& currentBricks = gs.grids[0].currentBricks;
							if (currentBricks[currentBricks.size() - 1].resets < 15)
							{
								currentBricks[currentBricks.size() - 1].resets += 1;
								gs.fallLockInTimer.reset();
							}
						}
					}
				}
				if (event.key.scancode == gs.configuratedKeys.softDrop)
				{
					gs.gravityTimer.updateLengthMeasure(0.02f);
				}
				if (event.key.scancode == gs.configuratedKeys.reserve)
				{
					GridData& gridData = gs.grids[0];

					Piece toBeCreatedPiece;
					Piece& reservedBrick = gridData.reservedBrick;
					std::vector<BrickData>& currentBricks = gridData.currentBricks;

					if (reservedBrick == Piece::nullPiece)
					{
						toBeCreatedPiece = gridData.nextBricks[gridData.rotationIndexNextBricks];
						stepNextBricks(gs, res, gridData, 0);
					}
					else
					{
						toBeCreatedPiece = reservedBrick;
					}


					reservedBrick = currentBricks.back().shape;


					for (GameObject& u : currentBricks.back().units)
					{
						BrickUnitData& data = gridData.currentUnits[u.specificDataLocation];
						data.shape = Piece::nullPiece;
					}

					currentBricks.back().numDreprecatedUnits = 4;
					currentBricks.back().hasToUpdate = true;

					gridData.bricksToUpdate.push_back(currentBricks.size() - 1);

					updateGrid(gs, res, gridData, 0, true);


					createPlayingBrick(gs, res, currentBricks, 0, toBeCreatedPiece);
				}
				if (state.keys[gs.configuratedKeys.hardDrop])
				{
					if (!gs.hardDropPressed && gs.preventAcidentalHardDropTimer.isTimedOut())
					{
						updateBrickPositionTranslational(gs.grids[0].currentBricks.back(), gs.grids[0], glm::ivec2(0, gs.grids[0].biggestYFallForPlayingBrick), true);

						gs.fallLockInTimer.timeOutTimer();
						gs.preventAcidentalHardDropTimer.reset();
						gs.hardDropPressed = true;
						gs.grids[0].biggestYFallForPlayingBrick = 0;

					}
				}
				break;
			case SDL_EVENT_KEY_UP:
				if (event.key.scancode == gs.configuratedKeys.softDrop)
				{
					gs.gravityTimer.updateLengthMeasure(1.0f);
				}
				if (event.key.scancode == gs.configuratedKeys.hardDrop)
				{
					gs.hardDropPressed = false;
				}
				break;
			default:
				break;
			}
		}

		//update bricks
		for (size_t i = 0; i < gs.grids.size(); i++)
		{
			for (BrickData& brick : gs.grids[i].currentBricks)
			{
				updateBricks(state, gs, res, gs.grids[i], brick, i, deltaTime);

			}
		}

		for (size_t i = 0; i < gs.grids.size(); i++)
		{
			GridData& gridData = gs.grids[i];
			BrickData& playingBrickFirst = gridData.currentBricks.back();
			BrickData& previewBrick = gridData.previewBrick;

			BrickUnitData& dataPlaying = gridData.currentUnits[playingBrickFirst.units[0].specificDataLocation];
			BrickUnitData& dataPreview = gridData.currentUnits[previewBrick.units[0].specificDataLocation];

			bool playingBrickMoved = dataPlaying.position != dataPlaying.prevPosition || dataPlaying.rotationState != dataPreview.rotationState;


			if (i > 0)
			{
				updateGrid(gs, res, gs.grids[i], i, true);
			}
			else
			{
				std::vector<std::vector<GridCell>>& grid = gs.grids[0].gridUnitsData;
				std::vector<BrickData>& currentBricks = gs.grids[0].currentBricks;

				gs.coutingLockIn = false;

				// for unit in current playing brick
				for (GameObject& u : currentBricks.back().units)
				{
					BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];


					int& y = uData.position.y;
					int& x = uData.position.x;

					if (y == 25) {
						gs.coutingLockIn = true;
						break;
					}
					else if ((grid[static_cast<size_t>(y + 1)][static_cast<size_t>(x)] && grid[static_cast<size_t>(y + 1)][static_cast<size_t>(x)].brickIndex != uData.indexInCurrentBricks)) {
						gs.coutingLockIn = true;
						break;
					}

				}

				if (!gs.coutingLockIn)
				{
					gs.fallLockInTimer.reset();
				}

				if (gs.fallLockInTimer.isTimedOut())
				{
					gs.fallLockInTimer.reset();
					gs.coutingLockIn = false;

					if (gs.grids[0].currentBricks.back().hasToUpdate)
					{
						gs.grids[0].currentBricks.back().hasToUpdate = false;

						auto playingBrickIt = std::max_element(gs.grids[0].bricksToUpdate.begin(), gs.grids[0].bricksToUpdate.end());

						gs.grids[0].bricksToUpdate.erase(playingBrickIt);
					}

					updateGrid(gs, res, gs.grids[i], i, true);

					playingPieceDropped(gs, res, gs.grids[0], 0);
				}
				else
				{
					updateGrid(gs, res, gs.grids[i], i, true);
				}
			}
			BrickData& playingBrickFinal = gridData.currentBricks.back();


			bool playingBrickChanged = playingBrickFinal.brickId != playingBrickFirst.brickId || dataPreview.position.x == -1;

			if (playingBrickChanged || playingBrickMoved)
			{
				gridData.biggestYFallForPlayingBrick = getbiggestYFallForBrick(playingBrickFinal, gridData);
				for (size_t i = 0; i < 4; i++)
				{
					BrickUnitData& finalUdata = gridData.currentUnits[playingBrickFinal.units[i].specificDataLocation];
					BrickUnitData& previewUdata = gridData.currentUnits[previewBrick.units[i].specificDataLocation];

					previewUdata.position = finalUdata.position;
					previewUdata.rotationState = finalUdata.rotationState;
				}
				std::cout << "aasjdfkjsdafkaaaaadjfjdfa\n";

				std::cout << "bggst fall: " << static_cast<int>(gridData.biggestYFallForPlayingBrick) << "\n";

				updateBrickPositionTranslational(previewBrick, gridData, glm::ivec2(0, gridData.biggestYFallForPlayingBrick));
			}

		}

		gs.FPSTimer.step(deltaTime);
		fps += 1;

		if (gs.FPSTimer.isTimedOut())
		{
			gs.FPSTimer.reset();
			gs.fps = fps;
			fps = 0;
		}

		std::cout << "FPS: " << gs.fps * 10 << "\n";

		// Clear the screen first
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Light& l : res.scenarioLights)
		{
			res.points[0].translateModel(l.position + glm::vec4(0.0f, 0.0f, 0.1f, 0.0f));
			res.points[0].Draw(res.shaderProgram[2], res.camera);
			res.points[0].translateModel(-l.position - glm::vec4(0.0f, 0.0f, 0.1f, 0.0f));
		}

		glBindBuffer(GL_UNIFORM_BUFFER, res.brickUBO);



		// Update camera matrix
		res.camera.updateMatrix(80.0f, 0.1f, 100.0f, state.width, state.height);

		res.anims[0][0].step(deltaTime);


		glBufferSubData(
			GL_UNIFORM_BUFFER,
			0,
			res.brickLights.size() * sizeof(Light),
			res.brickLights.data()
		);
		glBindBuffer(GL_UNIFORM_BUFFER, res.scenarioUBO);

		glBufferSubData(
			GL_UNIFORM_BUFFER,
			0,
			res.scenarioLights.size() * sizeof(Light),
			res.scenarioLights.data()
		);

		res.shaderProgram[1].Activate();
		glUniform1i(glGetUniformLocation(res.shaderProgram[1].ID, "numScenarioLights"), res.scenarioLights.size());

		for (size_t i = 0; i < gs.grids.size(); i++)
		{
			if (i == 0)
			{
				drawGrid(state, res, gs, gs.grids[i], deltaTime, i);
			}
		}

		res.shaderProgram[0].Activate();
		glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numBrickLights"), res.brickLights.size());
		glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numScenarioLights"), res.scenarioLights.size());
		
		glUniform2f(glGetUniformLocation(res.shaderProgram[0].ID, "texCoordOffset"), 0.0f, 0.0f);

		gs.scenarioObjects[0].Draw(res, res.shaderProgram[0], res.camera);

		glUniform2f(glGetUniformLocation(res.shaderProgram[0].ID, "texCoordOffset"), res.anims[0][0].getSpriteUVOffsetX(), res.anims[0][0].getSpriteUVOffsetY());

		// Draw the portal mesh
		gs.scenarioObjects[1].Draw(res, res.shaderProgram[0], res.camera);

		SDL_GL_SwapWindow(state.window);

		prevTime = nowTime;
	}

	cleanup(state);
	for (Mesh& mesh : res.meshs)
	{
		mesh.Delete();
	}
	res.shaderProgram[0].Delete();
	res.shaderProgram[1].Delete();

	return 0;
}

SDLState initialize(int width, int height)
{
	bool initSuccess = true;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		initSuccess = false;
	}

	// Atributos OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // <--- Adicionado para o Depth Test funcionar

	// Janela
	SDL_Window* window = SDL_CreateWindow("Tetris Roguelike", width, height, SDL_WINDOW_OPENGL);
	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", nullptr);
		initSuccess = false;
	}

	// Contexto
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating OpenGL Context", nullptr);
		initSuccess = false;
	}

	SDL_GL_MakeCurrent(window, context);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize GLAD", nullptr);
		initSuccess = false;
	}

	// Recursos e Estados
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDLState state(width, height, width, height, window, context, initSuccess);

	return state;
}

void cleanup(SDLState& state)
{
	SDL_GL_DestroyContext(state.context);
	SDL_DestroyWindow(state.window);
	SDL_Quit();
}


void drawGrid(SDLState& state, Resources& res, GameState& gs, GridData& gridData, float deltaTime, size_t gridIndex)
{
	std::vector<float> instancesData;

	int instanceCount = 4;

	for (int y = 0; y < gridData.gridRows; y++)
	{
		for (int x = 0; x < gridData.gridColumns; x++)
		{
			GridCell& cell = gridData.gridUnitsData[y][x];
			if (cell)
			{
				GameObject& unit = gridData.currentBricks[cell.brickIndex].units[cell.unitNum];
				BrickUnitData& uData = gridData.currentUnits[unit.specificDataLocation];


				instanceCount++;

				instancesData.push_back(x * 0.030483870967741);

				if (uData.indexInCurrentBricks == gridData.currentBricks.size() - 1 && !gs.coutingLockIn)
				{
					instancesData.push_back((gridData.gridRows - (y + gs.gravityTimer.getTime() / gs.gravityTimer.getLength()) - 1) * 0.030483870967741f * dsin(82.0f));

				}
				else
				{
					instancesData.push_back((gridData.gridRows - y - 1) * 0.030483870967741f * dsin(82.0f));

				}
				instancesData.push_back(0.0f);
				instancesData.push_back((static_cast<int>(uData.shape) + 1) * 0.029296875f);

				instancesData.push_back(0.0f);
			}
		}
	}

	GameObject& previewBrickU = gridData.previewBrick.units[0];
	GameObject& PlayingBrickU = gridData.currentBricks.back().units[0];
	BrickUnitData& previewBrickUData = gridData.currentUnits[previewBrickU.specificDataLocation];
	BrickUnitData& playingBrickUData = gridData.currentUnits[PlayingBrickU.specificDataLocation];

	if (previewBrickUData.position.y != playingBrickUData.position.y)
	{

		instanceCount += 4;
		glm::vec4 lightPos(0);

		switch (gridData.currentBricks.back().shape)
		{
		case Piece::O:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#fff405"), 1.0f);
			break;
		case Piece::S:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#6305ff"), 1.0f);

			break;
		case Piece::L:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#04ffc4"), 1.0f);

			break;
		case Piece::J:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#ff4305"), 1.0f);

			break;
		case Piece::I:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#fff405"), 1.0f);

			break;
		case Piece::T:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#3caa90"), 1.0f);

			break;
		case Piece::Z:
			res.brickLights[0].color = glm::vec4(res.hexadecimalToRGB("#3caa90"), 1.0f);

			break;
		default:
			break;
		}
		
		for (GameObject& unit : gridData.previewBrick.units)
		{
			BrickUnitData& uData = gridData.currentUnits[unit.specificDataLocation];

			int& x = uData.position.x;
			int& y = uData.position.y;

			std::cout << x << ", " << y;

			float worldSpaceGridAlignedX = x * 0.030483870967741f;
			float worldSpaceGridAlignedY = (gridData.gridRows - y - 1) * 0.030483870967741f;

			instancesData.push_back(worldSpaceGridAlignedX);
			instancesData.push_back(worldSpaceGridAlignedY);
			instancesData.push_back(0.0f);
			instancesData.push_back(9.0f * 0.029296875f);
			instancesData.push_back(0.0f);

			lightPos.x += worldSpaceGridAlignedX / 4.0f;
			lightPos.y += worldSpaceGridAlignedY / 4.0f;
		}
		
		res.brickLights[0].position = lightPos;
		
	}
	Piece& shape = gridData.nextBricks[gridData.rotationIndexNextBricks];
	std::array<glm::ivec2, 4> positions = createStartingPos(shape);
	for (glm::ivec2& pos : positions)
	{
		float x = pos.x - positions[0].x;
		float y = - (pos.y - positions[0].y);
	
		switch (shape)
		{
			case Piece::T:
				x += 1;
				y += 0.5;
				break;
			case Piece::I:
				x += 0.5;
				y += 1;
				break;
			case Piece::S:
				x += 1;
				y += 0.5;
				break;
			case Piece::Z:
				x += 1;
				y += 1.5;
				break;
			case Piece::J:
				x += 1;
				y += 1.5;
				break;
			case Piece::L:
				x += 1;
				y += 0.5;
				break;
			case Piece::O:
				x += 1.5;
				y += 1.5;
				break;
		}

		float worldSpaceGridAlignedX = 0.345161290322580f + x * 0.030483870967741f;
		float worldSpaceGridAlignedY = 0.291935483870967f + y * 0.030483870967741f;

		instancesData.push_back(worldSpaceGridAlignedX);
		instancesData.push_back(worldSpaceGridAlignedY);
		instancesData.push_back(0.0f);
		instancesData.push_back((static_cast<int>(shape) + 1) * 0.029296875f);
		instancesData.push_back(0.0f);
	}

	Piece& shapeHold = gridData.reservedBrick;
	if (shapeHold != Piece::nullPiece)
	{
		instanceCount += 4;
		std::array<glm::ivec2, 4> positionsHold = createStartingPos(shapeHold);
		for (glm::ivec2& pos : positionsHold)
		{
			float x = pos.x - positionsHold[0].x;
			float y = -(pos.y - positionsHold[0].y);

			switch (shapeHold)
			{
			case Piece::T:
				x += 1;
				y += 0.5;
				break;
			case Piece::I:
				x += 0.5;
				y += 1;
				break;
			case Piece::S:
				x += 1;
				y += 0.5;
				break;
			case Piece::Z:
				x += 1;
				y += 1.5;
				break;
			case Piece::J:
				x += 1;
				y += 1.5;
				break;
			case Piece::L:
				x += 1;
				y += 0.5;
				break;
			case Piece::O:
				x += 1.5;
				y += 1.5;
				break;
			}

			float worldSpaceGridAlignedX = -0.196774193548387f + x * 0.030483870967741f;
			float worldSpaceGridAlignedY = 0.464516129032258f + y * 0.030483870967741f;

			instancesData.push_back(worldSpaceGridAlignedX);
			instancesData.push_back(worldSpaceGridAlignedY);
			instancesData.push_back(0.0f);
			instancesData.push_back((static_cast<int>(shapeHold) + 1) * 0.029296875f);
			instancesData.push_back(0.0f);
		}
	}

	res.meshs[2].updateInstanceVBO(instancesData);
	res.meshs[2].Draw(res.shaderProgram[1], res.camera, glm::mat4(1), instanceCount);
}

/*void updateUnitInGridStatisticsAddition(GameState& gs, GridData& gridData, GameObject& u)
{
	int& X = u.data.brickUnit.position.x;
	int& Y = u.data.brickUnit.position.y;

	RowStatistics& row = gridData.rowsStatus[Y];

	// adds unit to new row if it is not deprecated
	row.positionsFilled[X] += 1;
	row.numOfBricks += 1;

	// updates new column height
	if (gridData.gridRows - Y > gridData.columnsHeight[X])
	{
		gridData.columnsHeight[X] = static_cast<int8_t>(gridData.gridRows - Y);
	}

	if (!row.hasToUpdate)
	{
		row.hasToUpdate = true;
		gridData.rowsToUpdate.push_back(Y);
	}

}
void updateUnitInGridStatisticsRemoval(GameState& gs, GridData& gridData, GameObject& u)
{
	int& X = u.data.brickUnit.position.x;
	int& Y = u.data.brickUnit.position.y;
	int& prevX = u.data.brickUnit.prevPosition.x;
	int& prevY = u.data.brickUnit.prevPosition.y;

	RowStatistics& prevRow = gridData.rowsStatus[prevY];

	// removes unit from prev row
	prevRow.positionsFilled[prevX] -= 1;
	prevRow.numOfBricks -= 1;

	// if new column differs from prev column and the old Y gave column height, update old column height
	if (prevX != X && gridData.gridRows - prevY == gridData.columnsHeight[prevX])
	{
		for (int i = gridData.gridRows - prevY; i < gridData.gridRows; i++)
		{
			if (gridData.gridUnitsData[i][prevX])
			{
				gridData.columnsHeight[prevX] -= i;
				break;
			}
		}
	}

	if (!prevRow.hasToUpdate)
	{
		prevRow.hasToUpdate = true;
		gridData.rowsToUpdate.push_back(Y);
	}
}
void updateGridStatistics(GameState& gs, GridData& gridData)
{
	if (gridData.rowsToUpdate.empty()) return;

	std::vector<std::array<int8_t, 3>> columnsCheckedForGaps;
	columnsCheckedForGaps.resize(gridData.gridColumns);

	for (size_t index : gridData.rowsToUpdate)
	{
		RowStatistics& row = gridData.rowsStatus[index];

		// updates aligning gaps if needed
		if (row.numOfBricks == gridData.gridColumns - 1)
		{
			int sum = 0;
			for (int i = 0; i < row.positionsFilled.size(); i++)
			{
				if (row.positionsFilled[i] > 0)
				{
					sum += i;
				}
			}
			// sum of a full line    [(num of terms / 2) * (first term + last term)] minus computedSum
			int position = (gridData.gridColumns / 2.0f) * (gridData.gridColumns - 1) - sum;

			int consecutiveGaps = 1;
			bool counting = true;
			for (int i = -1; i <= 1; i += 2)
			{
				amount
				while (counting)
				{
					int cord = row.y + consecutiveGaps*
					bool found = gridData.rowsStatus[].numOfBricks == gridData.gridColumns - 1 && gridData.rowsStatus[row.y - consecutiveGaps].positionsFilled[position];
					consecutiveGaps += 1;
				}
			}
		}






		row.hasToUpdate = false;
	}
}*/




