#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/Pietrino.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool PietrinoRegistered = (AsteroidFactory::registerAsteroid<Pietrino>(), true);

void Pietrino::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/Pietrino_Frag.spv");
	model.addTexture("textures/asteroids/rock/Albedo.jpg");
	model.addTexture("textures/asteroids/rock/MRAO.jpg");
	model.addTexture("textures/asteroids/rock/Normal.jpg");
}

std::string Pietrino::getModelName() {
	return "models/myasteroid.gltf";
}