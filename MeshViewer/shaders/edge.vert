#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};

void main(void)
{
    gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0f);
    gl_Position.z = gl_Position.z * 0.999f;
    gl_Position = projectionMatrix * gl_Position;
}
