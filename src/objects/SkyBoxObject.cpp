#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/objects/SkyBoxObject.hpp"
#include "../../headers/engine/scenes/Scene.hpp"

#include <ctime>

SkyBoxObject::SkyBoxObject(Camera c) {
	transform = Transform::identity();
	camera = c;
}

void SkyBoxObject::Instantiate() {
	//Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
    vertexDescriptor->addBinding(sizeof(SkyBoxObjectVertex), true);
    vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SkyBoxObjectVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(SkyBoxObjectVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SkyBoxObjectVertex, norm), sizeof(glm::vec3), NORMAL);
	setModel("models/SkyBoxCube.obj", vertexDescriptor);
	model.setShader("shaders/SkyBox_Vert.spv", "shaders/SkyBox_Frag.spv");
	const std::vector<std::string> textures = {"textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
}

void SkyBoxObject::Start() {
}

void SkyBoxObject::Update() {
}