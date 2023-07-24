#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/SpecialPietrinoBlu.hpp"
#include "../../../headers/objects/SpaceshipObject.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool SpecialPietrinoBluRegistered = (AsteroidFactory::registerSpecialAsteroid<SpecialPietrinoBlu>(), true);

void SpecialPietrinoBlu::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.addUniformData(&additionalUniforms, sizeof(SpecialPietrinoBluUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	// special pietrino color
	additionalUniforms.color = glm::vec4(0.1f, 0.3f, 0.8f, 0.95f);

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/SpecialPietrino_Frag.spv");
	model.addTexture("textures/asteroids/hoshi/CrystalNormalMapStar.png");
	model.addTexture("textures/asteroids/hoshi/NoiseStar.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
						  "textures/sky/top.png",   "textures/sky/bottom.png",
						  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);

	model.isTransparent = true;
}

std::string SpecialPietrinoBlu::getModelName() {
	return "models/Hoshi.gltf";
}

void SpecialPietrinoBlu::die() {
	((SpaceshipObject*)player)->setEffect(EFFECT_DAMAGE);
	AbstractAsteroidObject::die();
}

void SpecialPietrinoBlu::addScore(int points) {
	// it's special, you won't get points from it :(
}