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

layout(set = 1, binding = 1) uniform sampler2D albedo;
layout(set = 1, binding = 2) uniform sampler2D normals;
layout(set = 1, binding = 3) uniform samplerCube skybox;

float specGamma = 600;
float roughness = 0.85;

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));
	vec3 Bitan = cross(Norm, Tan) * fragTan.w;
	mat3 tbn = mat3(Tan, Bitan, Norm);
	vec3 nMap = (texture(normals, fragUV).rgb * 2 - 1);
	nMap.xy *= 1.2;
	vec3 N = normalize(tbn * nMap);
	
	vec3 albedoCol = texture(albedo, fragUV).rgb;
	
	vec3 V = normalize(gubo.eyePos - fragPos);
	
	// directional light
	vec3 L = gubo.directionalLightDirection;
	vec3 lightColor = gubo.directionalLightColor.rgb;
	vec3 diffuse = LambertDiffuse(albedoCol, L, N);
	vec3 specular = BlinnSpecular(V, N, L, lightColor, specGamma);
	vec3 DiffSpec = (diffuse + specular) * DirectionalLight(L, lightColor);
	
	// point light
	L = normalize(gubo.pointLightPosition - fragPos);
	lightColor = gubo.pointLightColor.rgb;
	diffuse = LambertDiffuse(albedoCol, L, N);
	specular = BlinnSpecular(V, N, L, lightColor, specGamma);
	DiffSpec += (diffuse + specular) * PointLight(gubo.pointLightPosition, lightColor, fragPos, gubo.pointLightDecayFactor, gubo.pointLightTargetDistance);
	
	// spot light
	L = normalize(gubo.spotlightPosition - fragPos);
	lightColor = gubo.spotlightColor.rgb;
	diffuse = LambertDiffuse(albedoCol, L, N);
	specular = BlinnSpecular(V, N, L, lightColor, specGamma);
	DiffSpec += (diffuse + specular) * Spotlight(gubo.spotlightPosition, lightColor, gubo.spotlightDirection, fragPos, gubo.spotlightDecayFactor, gubo.spotlightTargetDistance, gubo.spotlightCosIn, gubo.spotlightCosOut);
	
	vec3 Ambient = sh(N) * albedoCol;

    //For cubemap reflections
	float F0 = pow(1 - roughness, 4);
	vec3 I = -normalize(gubo.eyePos - fragPos);
    vec3 R = reflect(I, N);
	vec3 reflectionColor = texture(skybox, R).xyz * F0;
	
	outColor = vec4(clamp(DiffSpec + Ambient + reflectionColor, 0.0, 1.0), 1.0f);
}