#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 modelMat;
	mat4 nMat;
	mat4 cameraMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec4 inTan;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec4 outTan;

layout(set = 2, binding = 0) uniform KillerPietrinoUniforms {
    float time;
} killerPietrinoUniforms;

float amplitude = 0.02;
float freq = 1.5;

void main() {
	vec3 inPos = inPosition;
	vec3 normPos = normalize(inPos);
	float latitude = acos(normPos.y);
	float longitude = atan(normPos.x, normPos.z);
	float offset = amplitude * sin(20*latitude + killerPietrinoUniforms.time) * cos(20*longitude + freq * killerPietrinoUniforms.time);//amplitude * sin(killerPietrinoUniforms.time * freq);
	inPos += offset * inNormal;
	gl_Position = ubo.mvpMat * vec4(inPos, 1.0);
	outPosition = (ubo.modelMat * vec4(inPos, 1.0)).xyz;
	outUV = inUV;
	outNormal = (ubo.nMat * vec4(inNormal, 0)).xyz;
	outTan = vec4((ubo.nMat * vec4(inTan.xyz, 0)).xyz, inTan.w);
}