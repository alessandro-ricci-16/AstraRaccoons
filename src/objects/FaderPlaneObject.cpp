#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/FaderPlaneObject.hpp"

void FaderPlaneObject::Instantiate() {
    // Load the model component
    ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(FaderPlaneVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(FaderPlaneVertex, pos), sizeof(glm::vec3), POSITION);

    std::vector<FaderPlaneVertex> verts = {{glm::vec3(-1, -1, 0)},
                                           {glm::vec3(-1, 1, 0)},
                                           {glm::vec3(1, -1, 0)},
                                           {glm::vec3(1, 1, 0)}};
    std::vector<uint32_t> indices = {0, 1, 2, 1, 3, 2};
    setModel(verts, indices, vertexDescriptor);
    model.setShader("shaders/FaderShader_Vert.spv", "shaders/FaderShader_Frag.spv");
    model.isTransparent = true;
    model.backfaceCullingOn = false;
    // Enable GUBOs -- REQUIRED if the shader uses them!
    acceptsGUBOs = false;
}

void FaderPlaneObject::Update() {}