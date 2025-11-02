#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 mesh;
uniform mat4 model;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    mat4 modelMesh = model * mesh;

    gl_Position = lightProjection * lightView * modelMesh * vec4(inPosition, 1.0f);
}