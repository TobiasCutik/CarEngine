#version 460 core

in vec2 texCoord;

uniform vec4 entityColor;
uniform bool bHasTexture;
uniform sampler2D diffuseTexture;

out vec4 fragColor;

void main()
{
    if (bHasTexture)
        fragColor = texture(diffuseTexture, texCoord);
    else
        fragColor = entityColor;
}