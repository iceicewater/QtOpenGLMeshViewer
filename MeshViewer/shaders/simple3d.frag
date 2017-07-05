#version 330 core

in vec3 TexCoords;

uniform samplerCube texture3d;

out vec4 color;

void main()
{
    color = texture(texture3d, TexCoords);
}
