#version 330 core

in vec2 TexCoords;

uniform sampler2D texture2d;
uniform int mode;
uniform bool hdr;
uniform float exposure;

out vec4 fColor;

const float offset = 1.0f / 300;

vec2 offsets[9] = vec2[](
    vec2(-offset, offset),  // top-left
    vec2(0.0f,    offset),  // top-center
    vec2(offset,  offset),  // top-right
    vec2(-offset, 0.0f),    // center-left
    vec2(0.0f,    0.0f),    // center-center
    vec2(offset,  0.0f),    // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f,    -offset), // bottom-center
    vec2(offset,  -offset)  // bottom-right
);

float sharpenKernel[9] = float[](
    -1.0f, -1, -1,
    -1.0f,  9, -1,
    -1.0f, -1.0f, -1
);

float blurKernel[9] = float[](
    1.0f / 16, 2.0f / 16, 1.0f / 16,
    2.0f / 16, 4.0f / 16, 2.0f / 16,
    1.0f / 16, 2.0f / 16, 1.0f / 16
);

float edgeDetectionKernel[9] = float[](
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
);

vec4 origin()
{
    vec4 color = texture(texture2d, TexCoords);
    return color;
}

vec4 inversion()
{
    vec4 color = 1.0f - texture(texture2d, TexCoords);
    color.a = 1.0f;
    return color;
}

vec4 grayscale()
{
    vec4 color = texture(texture2d, TexCoords);
    float average = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
    color = vec4(average, average, average, 1.0f);
    return color;
}

vec4 kernel(int mode)
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture2d, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0f);
    for(int i = 0; i < 9; i++)
    {
        if (mode == 0) col += sampleTex[i] * sharpenKernel[i];
        else if (mode == 1) col += sampleTex[i] * blurKernel[i];
        else if (mode == 2) col += sampleTex[i] * edgeDetectionKernel[i];
    }
    return vec4(col, 1.0f);
}

void main()
{
    if (mode == 0) fColor = origin();
    else if (mode == 1) fColor = inversion();
    else if (mode == 2) fColor = grayscale();
    else if (mode == 3) fColor = kernel(0);
    else if (mode == 4) fColor = kernel(1);
    else if (mode == 5) fColor = kernel(2);
    else if (mode == 9)
    {
        if (gl_FragCoord.x < 150.0f) fColor = inversion();
        else if (gl_FragCoord.x < 300.0f) fColor = grayscale();
        else if (gl_FragCoord.x < 450.0f) fColor = kernel(0);
        else if (gl_FragCoord.x < 600.0f) fColor = kernel(1);
        else if (gl_FragCoord.x < 750.0f) fColor = kernel(2);
        else fColor = origin();
    }
    if (hdr) fColor = vec4(1.0f) - exp(-fColor * exposure);
}
