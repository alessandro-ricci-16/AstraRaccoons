#version 450

layout(set = 0, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

vec4 baseColor = vec4(0.8, 0.8, 0.8, 1);
vec4 outlineColor = vec4(0.2, 0, 0, 1);
vec4 shadowColor = vec4(0, 0, 0, 0);

void main()
{
    vec4 Tx = texture(texSampler, fragTexCoord);
    outColor = vec4((baseColor.rgb * Tx.r) * baseColor.a + (outlineColor.rgb * Tx.g) * outlineColor.a + (shadowColor.rgb * Tx.b) * shadowColor.a, Tx.a);
}