#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
   FragPos = vec3(model * vec4(aPos, 1.0));
   gl_Position = camMatrix * model * vec4(aPos, 1.0);

   gl_PointSize = 10.0; // ✅ MUST be here
}