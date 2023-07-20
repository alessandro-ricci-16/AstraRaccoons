#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "library.glsl"

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

layout(set = 1, binding = 1) uniform sampler2D tex;

const float beta = 0.2f;
const float g = 1.5;

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);

	vec3 albedo = texture(tex, fragUV).rgb;
	//Directional Light
	vec3 dirLightDiffuse = LambertDiffuse(albedo, gubo.directionalLightDirection, Norm);
	vec3 dirLightSpecular = BlinnSpecular(gubo.eyePos, Norm, gubo.directionalLightDirection, albedo * gubo.directionalLightColor.xyz, 160);
	vec3 dirLightColor = (dirLightDiffuse + dirLightSpecular) * DirectionalLight(gubo.directionalLightDirection, gubo.directionalLightColor.xyz);
	//Point Light(s)
	vec3 pointLightDir = gubo.pointLightPosition - fragPos;
	vec3 pointLightDiffuse = LambertDiffuse(albedo, pointLightDir, Norm);
	vec3 pointLightSpecular = BlinnSpecular(gubo.eyePos, Norm, pointLightDir, albedo * gubo.pointLightColor.xyz, 160);
	vec3 pointLightColor = (pointLightDiffuse + pointLightSpecular) * PointLight(gubo.pointLightPosition, gubo.pointLightColor.xyz, fragPos, gubo.pointLightDecayFactor, gubo.pointLightTargetDistance);
	//Spotlight
	vec3 spotLightDir = gubo.spotlightPosition - fragPos;
	vec3 spotLightDiffuse = LambertDiffuse(albedo, spotLightDir, Norm);
	vec3 spotLightSpecular = BlinnSpecular(gubo.eyePos, Norm, spotLightDir, albedo * gubo.spotlightColor.xyz, 160);
	vec3 spotLightColor = (spotLightDiffuse + spotLightSpecular) * Spotlight(gubo.spotlightPosition, gubo.spotlightColor.xyz, gubo.spotlightDirection, fragPos, gubo.spotlightDecayFactor, gubo.spotlightTargetDistance, gubo.spotlightCosIn, gubo.spotlightCosOut);
	//Sum everything
	outColor = vec4(Norm, 1);//vec4(clamp(spotLightColor, 0, 1), 1);
}