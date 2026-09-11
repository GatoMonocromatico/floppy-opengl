#pragma once
// -----------------------------------------------------------------------------
// camera.h — view matrix + optional FPS yaw/pitch + upload to shaders
// -----------------------------------------------------------------------------
// orientation is a forward vector; position + orientation + up define the view.
// cameraMatrix is typically proj * view and is sent as one mat4 uniform.

#include "util/DebugLog.h"
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "render/shaderClass.h"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	float pitch = 0.0f;
	float yaw = -90.0f;

	float sensitivity = 0.5f;

	Camera(glm::vec3 position) :
		position(position)
	{
	};
	Camera() :
		// Pulled back to a room framing that holds the whole cabinet AND the
		// portal floating beside it. The previous default sat almost against the
		// monitor, which filled the screen with the playfield and put every other
		// object in the scene off camera. TR_CAMERA overrides this at startup.
		position(glm::vec3(0.0f, 0.1f, 1.60f))
	{
	};
	// Rebuild cameraMatrix from position/orientation and a perspective projection.
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane, int width, int height);
	// Same, but orthographic, framing `halfHeight` world units above and below the
	// centre. Used for the opponent-board pass: under perspective the brick cubes
	// near the frame's edges are seen at an angle, show their dark side faces and
	// clip the frustum, so edge cells looked blunt next to the middle ones. Under
	// ortho every cube presents exactly its front face, so the whole board is
	// uniformly sharp -- and it renders as flat 2D pieces without needing separate
	// flat geometry, which is what the cabinet is supposed to draw anyway.
	void updateMatrixOrtho(float halfHeight, float nearPlane, float farPlane, int width, int height);
	// Upload the cached cameraMatrix to a mat4 uniform by name (e.g. "camMatrix").
	void Matrix(Shader& shader, const char* uniform);
	// Rotate the forward vector around an axis (used for one-off camera tweaks).
	void rotateCamera(glm::vec3 axis, float angle);
};
