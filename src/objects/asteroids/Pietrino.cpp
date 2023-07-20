#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/Pietrino.hpp"

void Pietrino::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.setShader("shaders/asteroids/SimpleAsteroidShader_Vert.spv", "shaders/asteroids/SimpleAsteroidShader_Frag.spv");
	model.addTexture("textures/asteroids/copper/Albedo.png");
	model.addTexture("textures/asteroids/copper/RoughnessMetalness.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
}

std::string Pietrino::getModelName() {
	return "models/Sphere.gltf";
}