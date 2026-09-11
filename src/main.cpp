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
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "game/gameObject.h"
#include "util/Timer.h"
#include <cmath>

#include "render/animation.h"
#include "render/Mesh.h"
#include "render/point.h"
#include "render/Light.h"
#include "game/brickUtils.h"
#include "game/Resources.h"
#include "game/GameState.h"
#include "game/gameCore.h"
#include "util/DebugLog.h"


bool moveCamera = false;

SDLState initialize(int width, int height);
void cleanup(SDLState& state);
void drawGrid(SDLState& state, Resources& res, GameState& gs, GridData& gridData, float deltaTime, size_t gridIndex, Camera& camera, bool primaryGrid = true);
void renderOpponentBoard(SDLState& state, Resources& res, GameState& gs, float deltaTime);
void applyCameraOverride(Camera& camera);
void applyPortalOverride(Portal& portal);
void saveScreenshot(SDLState& state, const char* path);
//void updateUnitInGridStatistics(GameState& gs, GridData& gridData, GameObject& u, size_t thisGridIndex);


int main(int argc, char* argv[])
{
	flux::verbose = false; // flip to false to silence all debug output

	SDLState state = initialize(1600, 900);

	if (!state.successfullyInitialized)
	{
		cleanup(state);
		return 1;
	}

	float cameraSpeed = 2;

	// The room is dark: the portal's matter refracts whatever is behind it, so a
	// white void would leave it nothing to blend into. Matches the cabinet art.
	glClearColor(0.035f, 0.055f, 0.062f, 1.0f);

	// CREATE RESORCES DATA STRUCTURE AND INITIALIZE IT HERE (GROUP TEXTURES/ANIMATIONS)
	std::vector<Shader> shaders = { Shader("assets/shaders/default.vert", "assets/shaders/default.frag"), Shader("assets/shaders/default.vert", "assets/shaders/brickUnit.frag"), Shader("assets/shaders/point.vert", "assets/shaders/point.frag"), Shader("assets/shaders/portal.vert", "assets/shaders/portal.frag") };

	Resources res(shaders);
	
	res.load();

	applyCameraOverride(res.camera);
	applyPortalOverride(res.portal);

	// The portal condenses into existence rather than being simply present. Once
	// the attack system exists this is what a magical link opening looks like;
	// for now it runs once at startup, and M replays it.
	res.portal.beginMaterialisation();

	//setup game data
	GameState gs(res);

	// Build piece-shape lookup table used by the AI before the game loop.
	initAIPieceShapes();

	for (size_t i = 0; i < gs.numOfGrids; i++)
	{
		createPlayingBrick(gs, res, i, gs.grids[i].nextBricks[0]);
		stepNextBricks(gs, res, gs.grids[i], i);
	}

	int fps = 0;

	// Debug hook: TR_SHOT=<path> renders for TR_SHOT_AFTER seconds (default 2),
	// writes one PNG and exits. Makes "change the shader, look at the result" a
	// scriptable loop instead of a manual one, and captures exactly the frame the
	// engine drew rather than whatever a screenshot tool catches.
	const char* shotPath = std::getenv("TR_SHOT");
	float shotAfter = 2.0f;
	if (const char* a = std::getenv("TR_SHOT_AFTER")) shotAfter = static_cast<float>(atof(a));
	float shotElapsed = 0.0f;

	//starting game loop
	bool running = true;

	//time management
	uint64_t prevTime = SDL_GetTicks();
	while (running)
	{
		DBG_IF(flux::verbose, "LOOP START");
		uint64_t nowTime = SDL_GetTicks();
		float deltaTime = (nowTime - prevTime) / 1000.0f;

		SDL_Event event{ 0 };

		// mechanic phase

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
				// debug: replay the portal's materialisation transition
				if (event.key.scancode == SDL_SCANCODE_M)
				{
					res.portal.beginMaterialisation();
				}
				// debug: dump the current frame to a PNG
				if (event.key.scancode == SDL_SCANCODE_F12)
				{
					saveScreenshot(state, "screenshot.png");
				}
				// enable camera movement
				if (event.key.scancode == SDL_SCANCODE_O)
				{
					moveCamera = !moveCamera;
					SDL_SetWindowRelativeMouseMode(state.window, moveCamera);
				}
				// reset camera
				if (event.key.scancode == SDL_SCANCODE_P)
				{
					res.camera.yaw = -90.0f;
					res.camera.pitch = 0.0f;
					res.camera.orientation = glm::vec3(0.0f, 0.0f, -1.0f);
				}
				// camera movement
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
				if (event.key.scancode == SDL_SCANCODE_DOWN)
				{
					res.camera.position += glm::vec3(0.0f, 0.0f, 1.00f) * cameraSpeed * deltaTime;
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

				// resets lock in position timer of piece if it is moved 
				if (gs.coutingLockIn)
				{
					for (SDL_Scancode keyCode : gs.configuratedKeys.handlingKeys)
					{
						if (event.key.scancode == keyCode)
						{
							BrickData& playing = gs.grids[0].playingBrick();
							if (playing.resets < 15)
							{
								playing.resets += 1;
								gs.fallLockInTimer.reset();
							}
						}
					}
				}
				// eventic piece handling
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

					stdMat<GridCell>& grid = gridData.gridUnitsData;
					BrickData& heldBrick = gridData.playingBrick();
					reservedBrick = heldBrick.shape;

					for (GameObject& u : heldBrick.units)
					{
						BrickUnitData& data = gridData.currentUnits[u.specificDataLocation];
						grid[data.prevPosition.y][data.prevPosition.x] = GridCell();
						data.shape = Piece::nullPiece;
					}

					heldBrick.numDreprecatedUnits = 4;

					if (!heldBrick.hasToUpdate)
					{
						heldBrick.hasToUpdate = true;
						gridData.bricksToUpdate.push_back(static_cast<int16_t>(gridData.playingBrickHandle));
					}

					// Spawns the replacement and re-points playingBrickHandle at it;
					// the held brick is released by updateGrid once its units are gone.
					createPlayingBrick(gs, res, 0, toBeCreatedPiece);
				}
				if (state.keys[gs.configuratedKeys.hardDrop])
				{
					DBG_IF(flux::verbose, "TRYING HARD DROP");
					if (!gs.hardDropPressed && gs.preventAcidentalHardDropTimer.isTimedOut())
					{
						if(updateBrickPositionTranslational(gs.grids[0].playingBrick(), gs.grids[0], glm::ivec2(0, gs.grids[0].biggestYFallForPlayingBrick), true))
						{
							DBG_IF(flux::verbose, "SUCCEDED");
							gs.grids[0].previewHasToUpdate = true;
						}
						else
						{
							DBG_IF(flux::verbose, "FAILED");

						}

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
			}
		}
		// end of event poll

		// updates playing bricks
		for (size_t i = 1; i < gs.grids.size(); i++)
		{
			DBG_IF(flux::verbose, "INITIALIZING updatePlayingBrick");
			updatePlayingBrick(state, gs, res, gs.grids[i], i, deltaTime);
		}
		updatePlayingBrick(state, gs, res, gs.grids[0], 0, deltaTime);

		// updates grids
		for (size_t i = 0; i < gs.grids.size(); i++)
		{
			GridData& gridData = gs.grids[i];
			
			if (i > 0)
			{
				// Run AI analysis (incremental) and execute moves when ready.
				updateGrid(gs, res, gs.grids[i], i, true);
			}
			else
			{
				DBG_IF(flux::verbose, "PREPARING UPDATE GRID CALL");

				BrickData& previewBrick = gridData.previewBrick;
				stdMat<GridCell>& grid = gridData.gridUnitsData;
				std::vector<BrickData>& currentBricks = gridData.currentBricks;

				// Consume the flag now: it's set by updatePlayingBrick (on
				// successful move/rotation), by the hard-drop poll handling
				// above, and by createPlayingBrick on brick creation.
				bool updatePreview = gridData.previewHasToUpdate;
				gridData.previewHasToUpdate = false;

				// uses last computed gs.coutingLockIn
				if (gs.coutingLockIn)
				{
					gs.fallLockInTimer.step(deltaTime);
				}

				// redetermines gs.coutingLockIn
				gs.coutingLockIn = false;
				for (GameObject& u : gridData.playingBrick().units)
				{
					BrickUnitData& uData = gridData.currentUnits[u.specificDataLocation];

					int& y = uData.position.y;
					int& x = uData.position.x;

					if (y == 25) {
						gs.coutingLockIn = true;
						break;
					}
					else if ((grid[static_cast<size_t>(y + 1)][static_cast<size_t>(x)] && gridData.currentUnits[grid[static_cast<size_t>(y + 1)][static_cast<size_t>(x)].unitHandle].indexInCurrentBricks != uData.indexInCurrentBricks)) {
						gs.coutingLockIn = true;
						break;
					}
				}
				
				// uses recomputed gs.coutingLockIn
				if (!gs.coutingLockIn)
				{
					gs.fallLockInTimer.reset();
				}

				if (gs.fallLockInTimer.isTimedOut())
				{
					gs.fallLockInTimer.reset();
					gs.coutingLockIn = false;

					updatePreview = true;

					if (gridData.playingBrick().hasToUpdate)
					{
						gridData.playingBrick().hasToUpdate = false;

						// Drop this brick's own entry: playingPieceDropped writes its
						// cells directly. Previously this took the largest index in
						// bricksToUpdate, assuming the playing brick was always the
						// last one pushed -- with pooled handles that no longer holds.
						auto playingBrickIt = std::find(gridData.bricksToUpdate.begin(), gridData.bricksToUpdate.end(),
							static_cast<int16_t>(gridData.playingBrickHandle));

						if (playingBrickIt != gridData.bricksToUpdate.end())
						{
							gridData.bricksToUpdate.erase(playingBrickIt);
						}
					}

					// already updates the grid, and re-points playingBrickHandle
					playingPieceDropped(gs, res, gs.grids[0], 0);
				}
				else
				{
					updateGrid(gs, res, gs.grids[i], i, true);
				}

				if (updatePreview)
				{
					for (size_t i = 0; i < 4; i++)
					{
						BrickUnitData& playingUdata = gridData.currentUnits[gridData.playingBrick().units[i].specificDataLocation];
						BrickUnitData& previewUdata = gridData.currentUnits[previewBrick.units[i].specificDataLocation];

						previewUdata.position = playingUdata.position;
						previewUdata.rotationState = playingUdata.rotationState;
					}
					
					updateBrickPositionTranslational(previewBrick, gridData, glm::ivec2(0, gridData.biggestYFallForPlayingBrick));
				}
			}
		}

		DBGGRID_IF(flux::verbose, gs.grids[0]);
		DBG_IF(flux::verbose, "biggestFall", gs.grids[0].biggestYFallForPlayingBrick);

		// drawing phase

		// Pass 1: the opponent's board, rendered head-on into its own target.
		// This has to happen before the main pass because the portal samples it,
		// and it binds/unbinds its own framebuffer and viewport.
		renderOpponentBoard(state, res, gs, deltaTime);

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

		// The hand-drawn portal spawn animation. Nothing draws it at present -- the
		// portal materialises procedurally instead (see docs/Portal.md) -- but the
		// frames are kept stepping so re-wiring them stays a one-liner.
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
				DBG_IF(flux::verbose, "INITIALIZING drawGrid");
				drawGrid(state, res, gs, gs.grids[i], deltaTime, i, res.camera, true);
			}
		}
		
		// draw setup 1
		DBG_IF(flux::verbose, "CHANGING SHADER TO INDEX 0");
		res.shaderProgram[0].Activate();
		DBG_IF(flux::verbose, "SET UNIFORM numBrickLights");
		glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numBrickLights"), res.brickLights.size());
		DBG_IF(flux::verbose, "SET UNIFORM numScenarioLights");
		glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numScenarioLights"), res.scenarioLights.size());
		DBG_IF(flux::verbose, "SET UNIFORM texCoordOffset");
		glUniform2f(glGetUniformLocation(res.shaderProgram[0].ID, "texCoordOffset"), 0.0f, 0.0f);

		DBG_IF(flux::verbose, "CALLING DRAW UPON SCENARIO OBJECT 0");
		gs.scenarioObjects[0].Draw(res, res.shaderProgram[0], res.camera);
		
		// Pass 3: the portal, last of all.
		//
		// It samples the finished frame in order to refract what is behind it, so
		// the frame has to be snapshotted first -- everything drawn after this
		// point is simply not part of what the matter can bend.
		//
		// scenarioObjects[1] used to be drawn here: it carried the portal spawn
		// animation but pointed at meshIDX 2 (the brick-unit cube), so it rendered
		// a stray cube at the bottom of the playfield and the animation was never
		// visible. The portal replaces it.
		DBG_IF(flux::verbose, "CAPTURING SCENE FOR PORTAL REFRACTION");
		res.sceneCapture.capture(state.width, state.height);

		DBG_IF(flux::verbose, "DRAWING PORTAL");
		res.portal.stepTime(deltaTime);
		// Depth writes off: the portal is a transparent volume, and letting its
		// proxy quad write depth would have the quad occlude the glow it sprays
		// beyond the window rectangle.
		glDepthMask(GL_FALSE);
		// Premultiplied alpha. portal.frag composites its layers front-to-back and
		// emits an already-premultiplied colour; ordinary GL_SRC_ALPHA blending
		// would multiply it by alpha a second time and halve the whole effect.
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		res.portal.Draw(res.shaderProgram[3], res.camera,
			res.opponentBoard.colorTex, res.sceneCapture.texture,
			glm::vec2(static_cast<float>(state.width), static_cast<float>(state.height)));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);

		// Before the swap: glReadPixels reads the back buffer, which is the frame
		// that was just drawn.
		if (shotPath)
		{
			shotElapsed += deltaTime;
			if (shotElapsed >= shotAfter)
			{
				std::cout << "fps=" << (gs.fps * 10) << std::endl;
				saveScreenshot(state, shotPath);
				running = false;
			}
		}

		DBG_IF(flux::verbose, "SWAPPING DRAW BUFFERS");
		SDL_GL_SwapWindow(state.window);
		

		gs.FPSTimer.step(deltaTime);
		fps += 1;

		if (gs.FPSTimer.isTimedOut())
		{
			gs.FPSTimer.reset();
			gs.fps = fps;
			fps = 0;
		}

		DBG_IF(flux::verbose, "FPS", gs.fps * 10);

		prevTime = nowTime;

		while ((SDL_GetTicks() - prevTime) < 1000 / gs.fpsLimit)
		{
			auto t = prevTime;
		}
	}

	cleanup(state);
	for (Mesh& mesh : res.meshs)
	{
		mesh.Delete();
	}
	res.portal.Delete();
	res.opponentBoard.Delete();
	res.sceneCapture.Delete();
	for (Shader& shader : res.shaderProgram)
	{
		shader.Delete();
	}

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


// Renders one enemy grid head-on into an offscreen target, which the portal then
// paints onto its matter. Deliberately a PLAIN view of the board rather than a
// perspective-correct hole in space: per the design, the magic recreates a view
// of the other place, it does not physically join the two rooms -- so what you
// see does not shift with your head, and the effect stays legible from anywhere.
void renderOpponentBoard(SDLState& state, Resources& res, GameState& gs, float deltaTime)
{
	if (gs.grids.size() < 2) return;

	res.opponentBoard.Bind();

	// The other machine's screen, not the room: dark, slightly cool.
	glClearColor(0.02f, 0.055f, 0.06f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Orthographic, not perspective: under perspective the brick cubes near the
	// frame's edges are seen at an angle and show their dark side faces, so the
	// outer columns and rows read blunt next to the middle ones. Ortho presents
	// every cube's front face identically, which both fixes that and makes the
	// opponent's pieces render flat -- the cabinet is supposed to draw plain 2D
	// pieces, with 3D pieces being the magic layer on top.
	res.portalViewCamera.updateMatrixOrtho(res.portalViewHalfHeight, 0.1f, 100.0f,
		res.opponentBoard.width, res.opponentBoard.height);

	res.portal.scale = 0.5f;
	res.portal.position = glm::vec3(0.42, 0, 0.1);
	res.portal.setTransform();

	// The cabinet art first, framed on the playfield well. Without it the board is
	// a void: the well's border and backdrop live in the arcade texture, not in
	// any per-cell geometry, so drawing only the bricks shows floating pieces on
	// black. Drawing the machine is also the honest reading of the design -- what
	// the portal shows is the opponent's SCREEN, not an abstract board.
	res.shaderProgram[0].Activate();
	glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numBrickLights"), res.brickLights.size());
	glUniform1i(glGetUniformLocation(res.shaderProgram[0].ID, "numScenarioLights"), res.scenarioLights.size());
	glUniform2f(glGetUniformLocation(res.shaderProgram[0].ID, "texCoordOffset"), 0.0f, 0.0f);
	gs.scenarioObjects[0].Draw(res, res.shaderProgram[0], res.portalViewCamera);

	res.shaderProgram[1].Activate();
	glUniform1i(glGetUniformLocation(res.shaderProgram[1].ID, "numScenarioLights"), res.scenarioLights.size());

	drawGrid(state, res, gs, gs.grids[1], deltaTime, 1, res.portalViewCamera, false);

	Framebuffer::Unbind();
	glViewport(0, 0, state.width, state.height);
	glClearColor(0.035f, 0.055f, 0.062f, 1.0f);
}

// Debug hook: TR_CAMERA="x,y,z,yaw,pitch" repositions the camera at startup.
// Exists so the portal can be screenshotted from several angles without hand
// editing and rebuilding -- a 3D effect that is only ever checked head-on is a
// 3D effect that has not been checked. Ignored when unset.
void applyCameraOverride(Camera& camera)
{
	const char* spec = std::getenv("TR_CAMERA");
	if (!spec) return;

	float v[5] = { camera.position.x, camera.position.y, camera.position.z, camera.yaw, camera.pitch };
	int parsed = std::sscanf(spec, "%f,%f,%f,%f,%f", &v[0], &v[1], &v[2], &v[3], &v[4]);
	if (parsed < 3)
	{
		std::cerr << "TR_CAMERA: expected \"x,y,z[,yaw,pitch]\", got \"" << spec << "\"" << std::endl;
		return;
	}

	camera.position = glm::vec3(v[0], v[1], v[2]);
	if (parsed >= 5)
	{
		camera.yaw = v[3];
		camera.pitch = v[4];
		glm::vec3 direction;
		direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		direction.y = sin(glm::radians(camera.pitch));
		direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.orientation = glm::normalize(direction);
	}
}

// Debug hook: TR_PORTAL="x,y,z[,yaw,pitch,scale]" repositions and resizes the
// portal at startup. Sizing it is almost entirely a visual judgement, and this
// makes trying a size a rerun rather than a rebuild. Ignored when unset.
void applyPortalOverride(Portal& portal)
{
	const char* spec = std::getenv("TR_PORTAL");
	if (!spec) return;

	float v[6] = { portal.position.x, portal.position.y, portal.position.z,
	               portal.yawDegrees, portal.pitchDegrees, portal.scale };
	int parsed = std::sscanf(spec, "%f,%f,%f,%f,%f,%f", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
	if (parsed < 3)
	{
		std::cerr << "TR_PORTAL: expected \"x,y,z[,yaw,pitch,scale]\", got \"" << spec << "\"" << std::endl;
		return;
	}

	portal.position = glm::vec3(v[0], v[1], v[2]);
	if (parsed >= 4) portal.yawDegrees = v[3];
	if (parsed >= 5) portal.pitchDegrees = v[4];
	if (parsed >= 6) portal.scale = v[5];
	portal.setTransform();
	// Only the model matrix changed, so the proxy quad is still valid -- but call
	// create() anyway: it rebuilds only when the local dimensions actually differ.
	portal.create();
}

// Debug hook: writes the current back buffer to a PNG. Reads the framebuffer
// directly rather than relying on a window manager screenshot, so the capture is
// exactly one rendered frame with no compositor or focus involvement.
void saveScreenshot(SDLState& state, const char* path)
{
	const int w = state.width;
	const int h = state.height;
	std::vector<unsigned char> pixels(static_cast<size_t>(w) * h * 4);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

	// GL's origin is bottom-left, an image file's is top-left.
	const size_t stride = static_cast<size_t>(w) * 4;
	std::vector<unsigned char> flipped(pixels.size());
	for (int y = 0; y < h; y++)
	{
		std::memcpy(flipped.data() + static_cast<size_t>(y) * stride,
		            pixels.data() + static_cast<size_t>(h - 1 - y) * stride,
		            stride);
	}

	SDL_Surface* surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32,
		flipped.data(), static_cast<int>(stride));
	if (!surface)
	{
		std::cerr << "screenshot: " << SDL_GetError() << std::endl;
		return;
	}
	if (!IMG_SavePNG(surface, path))
	{
		std::cerr << "screenshot: " << SDL_GetError() << std::endl;
	}
	else
	{
		std::cout << "screenshot -> " << path << std::endl;
	}
	SDL_DestroySurface(surface);
}

void drawGrid(SDLState& state, Resources& res, GameState& gs, GridData& gridData, float deltaTime, size_t gridIndex, Camera& camera, bool primaryGrid)
{
	std::vector<float> instancesData;

	int instanceCount = 4;

	DBG_IF(flux::verbose, "DEFINING INSTANCE BUFFER");
	// buffers instace data for every piece unit in game 
	for (int y = 0; y < gridData.gridRows; y++)
	{
		for (int x = 0; x < gridData.gridColumns; x++)
		{
			GridCell& cell = gridData.gridUnitsData[y][x];
			if (cell)
			{
				MDBG_IF(flux::verbose, "CELL DETECTED AT", DBG_N("x", x), DBG_N("y", y));

				BrickUnitData& uData = gridData.currentUnits[cell.unitHandle];

				instanceCount++;

				// x displacement
				instancesData.push_back(x * 0.030483870967741);

				// y displacement
				if (uData.indexInCurrentBricks == gridData.playingBrickHandle && !gs.coutingLockIn)
				{
					instancesData.push_back((gridData.gridRows - (y + gs.gravityTimer.getTime() / gs.gravityTimer.getLength()) - 1) * 0.030483870967741f * dsin(82.0f));
				}
				else
				{
					instancesData.push_back((gridData.gridRows - y - 1) * 0.030483870967741f * dsin(82.0f));
				}

				// z displacement
				instancesData.push_back(0.0f);

				// texture step, based on shape
				instancesData.push_back((static_cast<int>(uData.shape) + 1) * 0.029296875f);
				instancesData.push_back(0.0f);

				MDBG_IF(flux::verbose, "PUSHED INSTANCE DATA TO BUFFER");
			}
		}
	}

	GameObject& previewBrickU = gridData.previewBrick.units[0];
	GameObject& PlayingBrickU = gridData.playingBrick().units[0];
	BrickUnitData& previewBrickUData = gridData.currentUnits[previewBrickU.specificDataLocation];
	BrickUnitData& playingBrickUData = gridData.currentUnits[PlayingBrickU.specificDataLocation];

	if (previewBrickUData.position.y != playingBrickUData.position.y)
	{
		MDBG_IF(flux::verbose, "DRAWING PREVIEW UNITS");
		instanceCount += 4;
		glm::vec4 lightPos(0);

		// brickLights[0] is the player's preview-piece light. The opponent pass
		// draws a different grid through this same function, so it must not
		// repoint that light at the enemy's piece.
		Light savedPreviewLight = res.brickLights[0];

		switch (gridData.playingBrick().shape)
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
			MDBG_IF(flux::verbose, "LOOP TROUGH PREVIEW UNITS");
			BrickUnitData& uData = gridData.currentUnits[unit.specificDataLocation];

			int& x = uData.position.x;
			int& y = uData.position.y;

			MDBG_IF(flux::verbose, DBG_N("x", x), DBG_N("y", y));

			float worldSpaceGridAlignedX = x * 0.030483870967741f;
			float worldSpaceGridAlignedY = (gridData.gridRows - y - 1) * 0.030483870967741f;

			instancesData.push_back(worldSpaceGridAlignedX);
			instancesData.push_back(worldSpaceGridAlignedY);
			instancesData.push_back(0.0f);
			instancesData.push_back(9.0f * 0.029296875f);
			instancesData.push_back(0.0f);

			lightPos.x += worldSpaceGridAlignedX / 4.0f;
			lightPos.y += worldSpaceGridAlignedY / 4.0f;
			MDBG_IF(flux::verbose, "ITERATION END");
		}
		
		res.brickLights[0].position = lightPos;

		if (!primaryGrid)
		{
			res.brickLights[0] = savedPreviewLight;
		}
	}

	MDBG_IF(flux::verbose, "DRAWING NEXT PIECE");
	// draws next pieces on the slot
	
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

	MDBG_IF(flux::verbose, "DRAWING HOLD PIECE");
	// draws hold piece
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

	MDBG_IF(flux::verbose, "UPDATING INSTANCE VBO");
	res.meshs[2].updateInstanceVBO(instancesData);

	MDBG_IF(flux::verbose, "CALLING DRAW UPON CONSTRUCTED MESH");
	res.meshs[2].Draw(res.shaderProgram[1], camera, glm::mat4(1), instanceCount);
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

	vecArr<int8_t, 3> columnsCheckedForGaps;
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




