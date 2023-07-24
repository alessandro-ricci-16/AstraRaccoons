#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "../library.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec4 fragTan;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 pointLightPosition;           	// position of the point light
    vec4 pointLightColor;              	// color of the point light
	float pointLightDecayFactor;		// decay factor
    float pointLightTargetDistance;		// target distance
    vec3 directionalLightDirection;    	// direction of the directional light
    vec4 directionalLightColor;        	// color of the directional light
    vec3 spotlightPosition;            	// postion of the spotlight
    vec3 spotlightDirection;           	// direction of the spotlight/directional light
    vec4 spotlightColor;               	// color of the light
	float spotlightDecayFactor;			// decay factor
    float spotlightTargetDistance;		// target distance
	float spotlightCosIn;
    float spotlightCosOut;
    vec3 eyePos;
} gubo;

layout(set = 1, binding = 1) uniform sampler2D emission;

layout(set = 2, binding = 0) uniform KillerPietrinoUniforms {
    float time;
} killerPietrinoUniforms;

void main() {
    vec2 scaledUVs = (fragUV) + vec2(killerPietrinoUniforms.time * 0.5, 0);

	vec3 Norm = normalize(fragNorm);
	
	vec3 emissionCol = texture(emission, scaledUVs).rgb * 1.8;

	vec3 Ambient = sh(Norm) * emissionCol;
	
	outColor = vec4(clamp((emissionCol + Ambient), 0.0, 1.0), 1.0f);
}