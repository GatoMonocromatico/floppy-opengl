#define GLM_ENABLE_EXPERIMENTAL
#include "render/Portal.h"
#include <glm/gtc/epsilon.hpp>
#include <cmath>
#include <vector>

namespace
{
	// Animation rates, in radians (or arbitrary units) per second. The ratios
	// between them are irrational, which is the whole point: the combined noise
	// field is quasi-periodic and never returns to an earlier state, so there is
	// no loop for the player to notice after a long session.
	constexpr double swirlRate   = 0.6180339887498949;  // 1/phi
	constexpr double driftRate   = 0.4142135623730951;  // sqrt(2) - 1
	constexpr double flickerRate = 1.7320508075688772;  // sqrt(3)

	// Phases are wrapped to this period before upload. Large enough that no
	// visible discontinuity survives the wrap (the noise hash is periodic in its
	// input at a much finer scale), small enough that a float32 uniform keeps
	// plenty of mantissa after hours of play. A raw seconds-since-start float
	// would visibly quantise the swirl after ~an hour.
	constexpr double phaseWrap = 1024.0;

	double wrapPhase(double v)
	{
		v = std::fmod(v, phaseWrap);
		return v < 0.0 ? v + phaseWrap : v;
	}
}

void Portal::create()
{
	const glm::vec2 qh = quadHalf();

	// Rebuild only when the local dimensions actually changed. The attribute
	// layout is unchanged, so re-uploading the buffer's contents is enough -- the
	// VAO still points at the same VBO.
	if (built && glm::all(glm::epsilonEqual(qh, builtQuadHalf, 1e-6f))) return;
	built = true;
	builtQuadHalf = qh;

	const float hx = qh.x;
	const float hy = qh.y;

	// position.xy in world units (local space), uv in [0,1] across the quad.
	std::vector<float> quad = {
		-hx, -hy, 0.0f, 0.0f, 0.0f,
		 hx, -hy, 0.0f, 1.0f, 0.0f,
		 hx,  hy, 0.0f, 1.0f, 1.0f,
		-hx,  hy, 0.0f, 0.0f, 1.0f,
	};
	std::vector<GLuint> idx = { 0, 1, 2, 2, 3, 0 };

	vao.Bind();
	vbo.createVBO(quad);
	ebo.createEBO(idx);

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Portal::stepTime(float deltaTime)
{
	swirlPhase   = wrapPhase(swirlPhase   + swirlRate   * deltaTime);
	driftPhase   = wrapPhase(driftPhase   + driftRate   * deltaTime);
	flickerPhase = wrapPhase(flickerPhase + flickerRate * deltaTime);

	if (materialiseDirection != 0.0f)
	{
		materialisation += materialiseDirection * (deltaTime / materialiseDuration);

		// Each end is only an end for the direction travelling towards it. Testing
		// both bounds regardless cancelled an opening transition on its very first
		// frame: deltaTime is 0 on frame one (prevTime is sampled immediately
		// before the loop), so materialisation was still exactly 0 and the
		// "reached the bottom" test fired against a portal that was on its way up.
		if (materialiseDirection > 0.0f && materialisation >= 1.0f)
		{
			materialisation = 1.0f;
			materialiseDirection = 0.0f;
		}
		else if (materialiseDirection < 0.0f && materialisation <= 0.0f)
		{
			materialisation = 0.0f;
			materialiseDirection = 0.0f;
		}
	}
}

void Portal::setTransform(glm::vec3 pos, float yaw, float pitch, float uniformScale)
{
	position = pos;
	yawDegrees = yaw;
	pitchDegrees = pitch;
	scale = uniformScale;
	setTransform();
}

void Portal::setTransform()
{
	model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, glm::radians(yawDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(pitchDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	// Uniform only -- see the header on why local space must stay isotropic.
	model = glm::scale(model, glm::vec3(scale));
}

void Portal::Draw(Shader& shader, Camera& camera, GLuint contentTex, GLuint backgroundTex,
                  const glm::vec2& viewportSize)
{
	shader.Activate();
	vao.Bind();

	// The fragment shader builds its ray in portal-local space, so the camera has
	// to come along for the ride. Inverting on the CPU keeps a matrix inverse out
	// of the per-fragment path.
	const glm::mat4 invModel = glm::inverse(model);
	const glm::vec3 camLocal = glm::vec3(invModel * glm::vec4(camera.position, 1.0f));

	camera.Matrix(shader, "camMatrix");
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader.ID, "camLocal"), 1, glm::value_ptr(camLocal));
	glUniform2f(glGetUniformLocation(shader.ID, "windowHalf"), windowHalf.x, windowHalf.y);
	glUniform1f(glGetUniformLocation(shader.ID, "matterRadius"), matterRadius);
	glUniform2f(glGetUniformLocation(shader.ID, "contentScale"), contentScale.x, contentScale.y);
	glUniform1f(glGetUniformLocation(shader.ID, "swirlPhase"), static_cast<float>(swirlPhase));
	glUniform1f(glGetUniformLocation(shader.ID, "driftPhase"), static_cast<float>(driftPhase));
	glUniform1f(glGetUniformLocation(shader.ID, "flickerPhase"), static_cast<float>(flickerPhase));
	glUniform1f(glGetUniformLocation(shader.ID, "materialisation"), materialisation);
	glUniform2f(glGetUniformLocation(shader.ID, "viewportSize"), viewportSize.x, viewportSize.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, contentTex);
	glUniform1i(glGetUniformLocation(shader.ID, "contentTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backgroundTex);
	glUniform1i(glGetUniformLocation(shader.ID, "backgroundTex"), 1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	vao.Unbind();
}

void Portal::Delete()
{
	if (!built) return;
	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	built = false;
}
