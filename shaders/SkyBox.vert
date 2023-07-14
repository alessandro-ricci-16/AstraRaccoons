#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 mvpMat;
	mat4 modelMat;
	mat4 cameraMat;
} ubo;

layout(location = 0) out vec3 fragTexCoord;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

void main()
{
    fragTexCoord = inPosition;
    vec4 pos = ubo.mvpMat * vec4(inPosition, 0.0);
    gl_Position = pos.xyww;
}

