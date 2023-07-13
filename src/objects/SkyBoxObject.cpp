#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/objects/SkyBoxObject.hpp"
#include "../../headers/engine/scenes/Scene.hpp"

#include <ctime>

SkyBoxObject::SkyBoxObject(Camera c) {
	camera = c;
}

void SkyBoxObject::Instantiate() {
	//Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(SkyBoxObject), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SkyBoxObjectVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(SkyBoxObjectVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SkyBoxObjectVertex, norm), sizeof(glm::vec3), NORMAL);
	setModel("models/SkyBoxCube.obj", vertexDescriptor);
	model.setShader("shaders/SkyBox_Vert.spv", "shaders/SkyBox_Frag.spv");
	const std::vector<std::string> textures = {"textures/sky/bkg1_right.png", "textures/sky/bkg1_left.png",
							  "textures/sky/bkg1_top.png",   "textures/sky/bkg1_bot.png",
							  "textures/sky/bkg1_front.png", "textures/sky/bkg1_back.png" };
	model.addCubicTexture(textures);
}

void SkyBoxObject::Start() {
}

void SkyBoxObject::Update() {
	transform.TranslateTo(camera.getCameraPosition());
	//std::cout << camera.getCameraPosition().x << " " << camera.getCameraPosition().y << " " << camera.getCameraPosition().z << "\n";
}