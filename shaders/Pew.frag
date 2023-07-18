#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform PewUniform {
	vec3 inColor;
} uniforms;
layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(uniforms.inColor, 1);
}