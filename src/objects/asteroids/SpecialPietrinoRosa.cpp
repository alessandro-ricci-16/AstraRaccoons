#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/SpecialPietrinoRosa.hpp"
#include "../../../headers/objects/SpaceshipObject.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool SpecialPietrinoRosaRegistered = (AsteroidFactory::registerSpecialAsteroid<SpecialPietrinoRosa>(), true);

void SpecialPietrinoRosa::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.addUniformData(&additionalUniforms, sizeof(SpecialPietrinoRosaUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	// special pietrino color
	additionalUniforms.color = glm::vec4(1.0f, 0.0f, 0.75f, 0.95f);

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/SpecialPietrino_Frag.spv");
	model.addTexture("textures/asteroids/kokoro/CrystalNormalMapHeart.png");
	model.addTexture("textures/asteroids/kokoro/NoiseHeart.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
						  "textures/sky/top.png",   "textures/sky/bottom.png",
						  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);

	model.isTransparent = true;
}

std::string SpecialPietrinoRosa::getModelName() {
	return "models/Kokoro.gltf";
}

void SpecialPietrinoRosa::die() {
	((SpaceshipObject*)player)->setLives(1); // adds one hp
	AbstractAsteroidObject::die();
}

void SpecialPietrinoRosa::addScore(int points) {
	// it's special, you won't get points from it :(
}