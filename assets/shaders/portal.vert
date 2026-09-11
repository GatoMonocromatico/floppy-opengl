#version 330 core
// -----------------------------------------------------------------------------
// portal.vert -- proxy geometry for the magical window
// -----------------------------------------------------------------------------
// Does almost nothing on purpose: the entire effect is raymarched per-fragment in
// the portal's LOCAL space, so all this stage has to do is hand the fragment its
// own local position. `model` must stay rotation + translation + uniform scale
// (see render/Portal.h) or the local-space ray directions below go wrong.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec3 vLocal;
out vec2 vUV;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    vLocal = aPos;
    vUV = aUV;
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}
