#version 460 core

in vec2 texCoord;

uniform sampler2D textBitmap;
uniform vec4 textColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.f, 1.f, 1.f, texture(textBitmap, texCoord).r) * textColor;
}