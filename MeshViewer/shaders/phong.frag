#version 330 core
in VS_OUT {
    vec3 fragPos;
    flat vec3 fragPos_flat;
    vec2 texCoord;
    vec3 normal;
    flat vec3 normal_flat;
    mat3 TBN;
} fs_in;

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

uniform bool flatShading;
uniform bool lockAmbientToDiffuse;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform bool useAmbientMap;
uniform bool useDiffuseMap;
uniform bool useSpecularMap;
uniform bool useNormalMap;

uniform bool activateSkybox;
uniform samplerCube skybox;

out vec4 fColor;

vec4 caculateLightColor(int i, vec4 ambient, vec4 diffuse, vec4 specular, vec3 normal)
{
    vec3 fragPos;
    if (flatShading) fragPos =  fs_in.fragPos_flat;
    else fragPos =  fs_in.fragPos;
    float ambiIntensity, diffIntensity, specIntensity;
    vec3 lightDir;
    if (light[i].type == 0)
    {
        lightDir = light[i].position_v - fragPos;
        float distance = length(lightDir);
        lightDir = normalize(lightDir);
        float attenuation;
        if (light[i].attenuationType == 0)
            attenuation = 1.0f;
        else if (light[i].attenuationType == 1)
            attenuation = 1.0f / (1.0f + 0.09f * distance);
        else if (light[i].attenuationType == 2)
            attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * (distance * distance));
        ambiIntensity = diffIntensity = specIntensity = attenuation;
    }
    else if (light[i].type == 1)
    {
        lightDir = normalize(-light[i].direction_v);
        ambiIntensity = diffIntensity = specIntensity = 1.0f;
    }
    else if (light[i].type == 2)
    {
        lightDir = normalize(light[i].position_v - fragPos);
        float theta = dot(lightDir, normalize(-light[i].direction_v));
        float epsilon = light[i].innerCutOff - light[i].outerCutOff;
        float intensity = clamp((theta - light[i].outerCutOff) / epsilon, 0.0, 1.0);
        ambiIntensity = 1.0f;
        diffIntensity = specIntensity = intensity;
    }
    vec3 viewDir = normalize(-fragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    ambient = light[i].ambient * ambient * ambiIntensity;

    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light[i].diffuse * (diff * diffuse) * diffIntensity;

//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    specular = light[i].specular * (spec * specular) * specIntensity;

    return (ambient + diffuse + specular);
}

void main()
{
    vec4 diffuse;
    if (useDiffuseMap)
        diffuse = texture(diffuseMap, fs_in.texCoord);
    else
        diffuse = materialDiffuse;

    vec4 ambient;
    if (lockAmbientToDiffuse)
        ambient = diffuse;
    else if (useAmbientMap)
        ambient = texture(ambientMap, fs_in.texCoord);
    else
        ambient = materialAmbient;

    vec4 specular;
    if (useSpecularMap)
        specular = texture(specularMap, fs_in.texCoord);
    else
        specular = materialSpecular;

    vec3 normal;
    if (flatShading) normal = normalize(fs_in.normal_flat);
    else if(useNormalMap)
    {
        normal = texture(normalMap, fs_in.texCoord).rgb;
        normal = normal * 2.0f - 1.0f;
        normal = normalize(fs_in.TBN * normal);
    }
    else normal = normalize(fs_in.normal);

    for (int i = 0; i < lightNumber; i++)
    {
        if (light[i].enable)
            fColor += caculateLightColor(i, ambient, diffuse, specular, normal);
    }

    if (activateSkybox)
    {
        vec3 reflectDir = reflect(fs_in.fragPos, normal);
        fColor += specular * texture(skybox, reflectDir);
    }
}
