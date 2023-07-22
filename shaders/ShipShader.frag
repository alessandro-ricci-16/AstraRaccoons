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

layout(set = 1, binding = 1) uniform sampler2D albedo;
layout(set = 1, binding = 2) uniform sampler2D metallicRoughnessEmission;
layout(set = 1, binding = 3) uniform samplerCube skybox;

layout(set = 2, binding = 0) uniform SpaceshipUniforms {
	vec4 flashingColor;
	vec4 emissionColor;
} spaceshipUniforms;

const float beta = 0.2f;
const float g = 1.5;

void main() {
	/*vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	
	// replace the following lines with the code to implement a point light model
	// with the light color in gubo.pointLightColor, and the position in gubo.pointLightPosition.
	// the exponent of the decay is in constant variable beta, and the base distance
	// is in constant g
	vec3 dir = gubo.pointLightPosition - fragPos;
	vec3 lightDir = normalize(dir);
	float decay = pow(g / length(dir), beta);
	vec3 pointLightColor = gubo.pointLightColor.xyz * decay;

	vec3 Diffuse = texture(albedo, fragUV).rgb * 0.99f * clamp(dot(Norm, lightDir),0.0,1.0);
	vec3 Specular = vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), 160.0f));
	vec3 Ambient = texture(albedo, fragUV).rgb * 0.01f;
	
	outColor = vec4(clamp((Diffuse + Specular) * pointLightColor.rgb + Ambient,0.0,1.0), 1.0f); //vec4(texture(albedo, fragUV).rgb, 1.0f);
	*/
	vec3 Norm = normalize(fragNorm);
	//vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));

	vec3 albedoCol = texture(albedo, fragUV).rgb;

	vec4 MRE = texture(metallicRoughnessEmission, fragUV);
	float roughness = MRE.g;
	float emission = MRE.b  * spaceshipUniforms.emissionColor.a;
	float metallic = MRE.r;
	
	vec3 L = gubo.directionalLightDirection;
	vec3 lightColor = gubo.directionalLightColor.rgb;

	vec3 V = normalize(gubo.eyePos - fragPos);

	vec3 DiffSpec = GGXDiffuseSpecular(V, Norm, L, albedoCol, 0.3f, metallic, roughness) * lightColor;
	vec3 Ambient = sh(Norm) * albedoCol;
	vec3 emissionColor = emission * spaceshipUniforms.emissionColor.rgb;
	
	//For cubemap reflections
	vec3 I = -normalize(gubo.eyePos - fragPos);
    vec3 R = reflect(I, Norm);
	float reflectivity = 1 - roughness;
	float F0 = pow(reflectivity, 4);
	vec3 reflectionColor = texture(skybox, R).xyz * F0;
	
	outColor = vec4(clamp((DiffSpec + Ambient) * (1 - emission) + emissionColor + reflectionColor + (spaceshipUniforms.flashingColor.xyz * spaceshipUniforms.flashingColor.w), 0.0, 1.0), 1.0f);
}