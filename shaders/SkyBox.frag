#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragTexCoord;

layout(set = 0, binding = 1) uniform samplerCube skybox;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(skybox, fragTexCoord);
}

