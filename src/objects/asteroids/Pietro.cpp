#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/Pietro.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool PietroRegistered = (AsteroidFactory::registerAsteroid<Pietro>(), true);

void Pietro::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/Pietro_Frag.spv");
	model.addTexture("textures/asteroids/paintedMetal/Albedo.jpg");
	model.addTexture("textures/asteroids/paintedMetal/MRAO.jpg");
	model.addTexture("textures/asteroids/paintedMetal/Normal.jpg");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
}

std::string Pietro::getModelName() {
	return "models/asteroid.gltf";
}