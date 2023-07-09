#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
	vec3 pointLightPosition;           // position of the point light
    vec4 pointLightColor;              // color of the point light
    vec3 directionalLightDirection;    // direction of the directional light
    vec4 directionalLightColor;        // color of the directional light
    vec3 spotlightPosition;            // postion of the spotlight
    vec3 spotlightDirection;           // direction of the spotlight/directional light
    vec4 spotlightColor;               // color of the light
    vec3 eyePos;
} gubo;

layout(set = 1, binding = 1) uniform sampler2D tex;

const float beta = 0.2f;
const float g = 1.5;

void main() {
	vec3 Norm = normalize(fragNorm);
	vec3 EyeDir = normalize(gubo.eyePos - fragPos);
	
	// replace the following lines with the code to implement a point light model
	// with the light color in gubo.pointLightColor, and the position in gubo.pointLightPosition.
	// the exponent of the decay is in constant variable beta, and the base distance
	// is in constant g
	vec3 dir = gubo.pointLightPosition - fragPos;
	vec3 lightDir = normalize(dir);
	float decay = pow(g / length(dir), beta);
	vec3 pointLightColor = gubo.pointLightColor.xyz * decay;

	vec3 Diffuse = texture(tex, fragUV).rgb * 0.99f * clamp(dot(Norm, lightDir),0.0,1.0);
	vec3 Specular = vec3(pow(clamp(dot(Norm, normalize(lightDir + EyeDir)),0.0,1.0), 160.0f));
	vec3 Ambient = texture(tex, fragUV).rgb * 0.01f;
	
	outColor = vec4(clamp((Diffuse + Specular) * pointLightColor.rgb + Ambient,0.0,1.0), 1.0f); //vec4(texture(tex, fragUV).rgb, 1.0f);
}