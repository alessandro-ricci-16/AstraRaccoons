#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 modelMat;
	mat4 cameraMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec3 outNormal_Reflections;
layout(location = 4) out vec3 outPosition_Reflections;

void main() {
	gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
	outPosition = gl_Position.xyz;
	outUV = inUV;
	outNormal = vec3(ubo.mvpMat * vec4(inNormal, 1.0));

	outPosition_Reflections = vec3(ubo.modelMat * mat4(vec4(100, 0, 0, 0), vec4(0, 100, 0, 0), vec4(0, 0, 100, 0), vec4(0, 0, 0, 1)) * vec4(inPosition, 0.0));
	outNormal_Reflections = mat3(transpose(inverse(ubo.modelMat))) * inNormal;
}