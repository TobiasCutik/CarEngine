#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 mesh;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;
uniform mat4 modelToWorldNormal;

out vec3 normal;
out vec3 fragmentPosition;
out vec4 fragmentPositionLightSpace;
out vec2 texCoord;
out vec3 vertexPosition;

void main()
{
    mat4 modelMesh = model * mesh;

    normal = mat3(modelToWorldNormal) * inNormal;
    fragmentPosition = vec3(modelMesh * vec4(inPosition, 1.f));
    fragmentPositionLightSpace = lightProjection * lightView * vec4(fragmentPosition, 1.f);
    texCoord = inTexCoord;
    vertexPosition = inPosition;

    gl_Position = projection * view * modelMesh * vec4(inPosition, 1.f);
}