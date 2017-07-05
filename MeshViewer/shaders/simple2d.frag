#version 330 core

in vec2 TexCoords;

uniform sampler2D texture2d;

out vec4 color;

void main()
{
    color = texture(texture2d, TexCoords);
}
