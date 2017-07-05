#version 330 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out VS_OUT {
    vec3 normal;
} vs_out;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    vs_out.normal = normalize(vec3(projectionMatrix * vec4(normalMatrix * normal, 1.0f)));
}
