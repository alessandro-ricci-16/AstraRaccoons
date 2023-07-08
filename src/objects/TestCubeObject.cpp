#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/objects/TestCubeObject.hpp"

void TestCubeObject::Instantiate() {
    //Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(TestCubeVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(TestCubeVertex, pos), sizeof(glm::vec3), POSITION);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(TestCubeVertex, uv), sizeof(glm::vec2), UV);
    setModel("models/Cube.obj", vertexDescriptor);
	model.setShader("shaders/Shader_Vert.spv", "shaders/Shader_Frag.spv");
	model.addTexture("textures/Checker.png");
}

void TestCubeObject::Start() {

}

void TestCubeObject::Update() {

}

void TestCubeObject::Destroy() {

}
