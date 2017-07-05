#version 330 core

layout (location = 0) in vec3 position;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};

out vec3 TexCoords;

void main()
{
    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(position, 1.0f);
    gl_Position = gl_Position.xyww;
    TexCoords = position;
}
