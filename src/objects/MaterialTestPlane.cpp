#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/MaterialTestPlane.hpp"

void TestPlaneObject::Instantiate() {
    // Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(TestPlaneVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT,
                                  offsetof(TestPlaneVertex, pos),
                                  sizeof(glm::vec3), POSITION);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT,
                                  offsetof(TestPlaneVertex, uv),
                                  sizeof(glm::vec2), UV);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT,
                                  offsetof(TestPlaneVertex, norm),
                                  sizeof(glm::vec3), NORMAL);
    setModel("models/Plane.obj", vertexDescriptor);
    model.setShader("shaders/Shader_Vert.spv", "shaders/Shader_Frag.spv");
    model.addTexture("textures/Checker.png");
    transform.Rotate(180, glm::vec3(1, 0, 0));
}

void TestPlaneObject::Start() {}

void TestPlaneObject::Update() {}

void TestPlaneObject::OnCollisionWith(GameObject* other) {}