#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/SpecialPietrinoVerde.hpp"
#include "../../../headers/objects/SpaceshipObject.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool SpecialPietrinoVerdeRegistered = (AsteroidFactory::registerSpecialAsteroid<SpecialPietrinoVerde>(), true);

void SpecialPietrinoVerde::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.addUniformData(&additionalUniforms, sizeof(SpecialPietrinoVerdeUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	// special pietrino color
	additionalUniforms.color = glm::vec4(0.1f, 0.5f, 0.1f, 0.95f);

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/SpecialPietrino_Frag.spv");
	model.addTexture("textures/asteroids/hoshi/CrystalNormalMapStar.png");
	model.addTexture("textures/asteroids/hoshi/NoiseStar.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
						  "textures/sky/top.png",   "textures/sky/bottom.png",
						  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);

	model.isTransparent = true;
}

std::string SpecialPietrinoVerde::getModelName() {
	return "models/Hoshi.gltf";
}

void SpecialPietrinoVerde::die() {
	((SpaceshipObject*)player)->setEffect(EFFECT_TIME);
	AbstractAsteroidObject::die();
}

void SpecialPietrinoVerde::addScore(int points) {
	// it's special, you won't get points from it :(
}