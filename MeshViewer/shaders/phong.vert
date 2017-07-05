#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};

struct LightStruct
{
    vec3 position;
    vec3 direction;
    vec3 position_v;
    vec3 direction_v;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    int type;
    int attenuationType;
    float innerCutOff;
    float outerCutOff;
    bool enable;
};

layout (std140) uniform Lights
{
    int lightNumber;
    LightStruct light[10];
};

out VS_OUT {
    vec3 fragPos;
    flat vec3 fragPos_flat;
    vec2 texCoord;
    vec3 normal;
    flat vec3 normal_flat;
    mat3 TBN;
} vs_out;

void main()
{
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);

    gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0f);
    vs_out.fragPos = vec3(gl_Position);
    vs_out.fragPos_flat = vec3(gl_Position);
    gl_Position = projectionMatrix * gl_Position;
    vs_out.texCoord = texCoord;
    vs_out.normal = normalMatrix * normal;
    vs_out.normal_flat = normalMatrix * normal;
}
