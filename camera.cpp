#define GLM_ENABLE_EXPERIMENTAL

#include"camera.h"

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane, int width, int height)
{
	glm::mat4 view = glm::lookAt(position, position + orientation, up);
	glm::mat4 proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
	cameraMatrix = proj * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

}

void Camera::rotateCamera(glm::vec3 axis, float angle)
{
	orientation = glm::rotate(orientation, angle, axis);
}