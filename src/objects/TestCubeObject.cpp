#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/objects/TestCubeObject.hpp"
#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"

#include <ctime>

void TestCubeObject::Instantiate() {
    //Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(TestCubeVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(TestCubeVertex, pos), sizeof(glm::vec3), POSITION);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(TestCubeVertex, uv), sizeof(glm::vec2), UV);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(TestCubeVertex, norm), sizeof(glm::vec3), NORMAL);
    setModel("models/Cube.obj", vertexDescriptor);
	model.setShader("shaders/Shader_Vert.spv", "shaders/Shader_Frag.spv");
	model.addTexture("textures/Checker.png");
    transform.TranslateTo(glm::vec3(0, 0, -1));
    //Add the collider
    setCollider(1, 1, 1);
    // Enable GUBOs -- REQUIRED if the shader uses them!
    acceptsGUBOs = true;
}

void TestCubeObject::Update() {
    transform.Rotate(20*Time::getDeltaT(), glm::vec3(1, 0, 0));
    if (moves) {
        transform.TranslateBy(glm::vec3(-0.01, 0, 0));
    }
}

void TestCubeObject::OnCollisionWith(GameObject* other) {
    parentScene->removeObject(this);
}