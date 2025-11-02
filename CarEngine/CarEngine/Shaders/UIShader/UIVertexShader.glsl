#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 mesh;
uniform mat4 model;
uniform mat4 uiProjection;

out vec2 texCoord;

void main()
{
    texCoord = inTexCoord;
    mat4 modelMesh = model * mesh;
    gl_Position = uiProjection * modelMesh * vec4(inPosition, 1.f);
}