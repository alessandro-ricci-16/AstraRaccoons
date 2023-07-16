#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "library.glsl"

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(vec3(0.1), 0.9);
}