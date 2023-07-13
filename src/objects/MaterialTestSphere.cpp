#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/MaterialTestSphere.hpp"

void TestSphereObject::Instantiate() {
    // Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(TestSphereVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT,
                                  offsetof(TestSphereVertex, pos),
                                  sizeof(glm::vec3), POSITION);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT,
                                  offsetof(TestSphereVertex, uv),
                                  sizeof(glm::vec2), UV);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT,
                                  offsetof(TestSphereVertex, norm),
                                  sizeof(glm::vec3), NORMAL);
    setModel("models/Sphere.gltf", vertexDescriptor);
    model.setShader("shaders/Shader_Vert.spv", "shaders/Shader_Frag.spv");
    model.addTexture("textures/Checker.png");
    // Enable GUBOs -- REQUIRED if the shader uses them!
    acceptsGUBOs = true;
}

void TestSphereObject::Start() {}

void TestSphereObject::Update() {
}

void TestSphereObject::OnCollisionWith(GameObject* other) {
}