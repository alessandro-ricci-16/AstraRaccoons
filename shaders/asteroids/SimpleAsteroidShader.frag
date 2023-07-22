#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "../library.glsl"

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

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

layout(set = 1, binding = 1) uniform sampler2D albedo;
layout(set = 1, binding = 2) uniform sampler2D metallicRoughness;
layout(set = 1, binding = 3) uniform samplerCube skybox;

const float beta = 0.2f;
const float g = 1.5;

void main() {
	vec3 Norm = normalize(fragNorm);
	//vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));

	vec3 albedoCol = texture(albedo, fragUV).rgb;

	vec4 MR = texture(metallicRoughness, fragUV);
	float roughness = MR.g;
	float metallic = MR.r;
	
	vec3 L = gubo.directionalLightDirection;
	vec3 lightColor = gubo.directionalLightColor.rgb;

	vec3 V = normalize(gubo.eyePos - fragPos);

	float reflectivity = 1 - roughness;
	float F0 = pow(reflectivity, 4);

	vec3 DiffSpec = GGXDiffuseSpecular(V, Norm, L, albedoCol, F0, metallic, roughness);
	
	//For cubemap reflections
	vec3 I = -normalize(gubo.eyePos - fragPos);
    vec3 R = reflect(I, Norm);

	vec3 reflectionColor = texture(skybox, R).xyz * F0;
	
	outColor = vec4(clamp(DiffSpec + reflectionColor, 0.0, 1.0), 1.0f);
}