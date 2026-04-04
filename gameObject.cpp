#include"gameObject.h"
#include"Resources.h"

void GameObject::translateModel(glm::vec3 translation)
{
	model = glm::translate(model, translation);
}

void GameObject::rotateModel(glm::vec3 axis, float angle)
{
	model = glm::rotate(model, angle, axis);
}

void GameObject::scaleModel(glm::vec3 scale)
{
	model = glm::scale(model, scale);
}
void GameObject::Draw(Resources& res, Shader& shader, Camera& camera)
{
	Mesh& mesh = res.meshs[meshIDX];
	VAO& VAO1 = mesh.VAO1;

	shader.Activate();
	VAO1.Bind();

	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;

	for (unsigned int i = 0; i < mesh.textures.size(); i++)
	{
		Texture& tex = mesh.textures[i];
		std::string number;
		std::string type = tex.type;
		if (type == "tex")
			number = std::to_string(diffuseNr++);
		else if (type == "mask")
			number = std::to_string(specularNr++);
		tex.texUnit(shader, (type + number).c_str(), i);
		glActiveTexture(tex.slot);
		tex.Bind();
	}

	camera.Matrix(shader, "camMatrix");

	glm::mat4 resultingModel = mesh.model * model;
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(resultingModel));

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void GameObject::Draw(Resources& res, Shader& shader, Camera& camera, int numInstances)
{
	Mesh& mesh = res.meshs[meshIDX];
	VAO& VAO1 = mesh.VAO1;

	shader.Activate();
	VAO1.Bind();

	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;

	for (unsigned int i = 0; i < mesh.textures.size(); i++)
	{
		Texture& tex = mesh.textures[i];
		std::string number;
		std::string type = tex.type;
		if (type == "tex")
			number = std::to_string(diffuseNr++);
		else if (type == "mask")
			number = std::to_string(specularNr++);
		tex.texUnit(shader, (type + number).c_str(), i);
		glActiveTexture(tex.slot);
		tex.Bind();
	}

	camera.Matrix(shader, "camMatrix");

	glm::mat4 resultingModel = mesh.model * model;
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(resultingModel));

	glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, numInstances);
}