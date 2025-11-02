#version 460 core

layout (location = 0) in vec3 inPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 texCoord;

void main()
{
    texCoord = inPosition;
    vec4 position = projection * view * model * vec4(inPosition, 1.f);
    gl_Position = position.xyww;
}