#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D source;
layout(binding = 2) uniform sampler2D gradient;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};


void main()
{
    vec4 srcColor = texture(source, qt_TexCoord0);
    vec4 gradientColor = texture(gradient, qt_TexCoord0);
    fragColor = vec4(gradientColor.rgb, 1.0) * srcColor.a * qt_Opacity;
}
