#define MESHOBJECT_IMPLEMENTATION
#include <headers/engine/base/libraries.hpp>
#include <headers/engine/base/Time.hpp>
#include <headers/engine/scenes/Scene.hpp>
#include <headers/objects/SpaceshipObject.hpp>

void SpaceshipObject::Instantiate() {
    // Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(SpaceshipVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SpaceshipVertex, pos), sizeof(glm::vec3), POSITION);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(SpaceshipVertex, uv), sizeof(glm::vec2), UV);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SpaceshipVertex, norm), sizeof(glm::vec3), NORMAL);
    setModel("models/spaceship.obj", vertexDescriptor);
    model.setShader("shaders/ShipShader_Vert.spv", "shaders/ShipShader_Frag.spv");
    model.addTexture("textures/spaceship/Albedo.png");
    model.addTexture("textures/spaceship/MetallicRoughnessEmission.png");
    transform.TranslateBy(glm::vec3(-3, 0, 0));
    transform.Scale(glm::vec3(0.3f));
}

void SpaceshipObject::Start() {}

void SpaceshipObject::Update() {
    transform.Rotate(40 * Time::getDeltaT(), glm::vec3(0, 1, 0));
}

void SpaceshipObject::OnCollisionWith(GameObject* other) {
    
}