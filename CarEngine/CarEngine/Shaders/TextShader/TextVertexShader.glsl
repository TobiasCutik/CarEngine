#version 460 core

layout (location = 0) in vec4 inTextQuadData; // <vec2 position, vec2 texCoord>

uniform mat4 uiProjection;
uniform mat4 model;

out vec2 texCoord;

void main()
{
    vec2 position = inTextQuadData.xy;
    texCoord = inTextQuadData.zw;
    gl_Position = uiProjection * model * vec4(position, 0.f, 1.f);
}