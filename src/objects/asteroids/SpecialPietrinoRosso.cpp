#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/SpecialPietrinoRosso.hpp"
#include "../../../headers/objects/SpaceshipObject.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool SpecialPietrinoRossoRegistered = (AsteroidFactory::registerSpecialAsteroid<SpecialPietrinoRosso>(), true);

void SpecialPietrinoRosso::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.addUniformData(&additionalUniforms, sizeof(SpecialPietrinoRossoUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	// special pietrino color
	additionalUniforms.color = glm::vec4(1.0f, 0.0f, 0.0f, 0.95f);

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv", "shaders/asteroids/SpecialPietrino_Frag.spv");
	model.addTexture("textures/asteroids/crystal/CrystalNormalMap.png");
	model.addTexture("textures/asteroids/crystal/Noise.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
						  "textures/sky/top.png",   "textures/sky/bottom.png",
						  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);

	model.isTransparent = true;
}

std::string SpecialPietrinoRosso::getModelName() {
	return "models/Hoshi.gltf";
}

void SpecialPietrinoRosso::die() {
	((SpaceshipObject*)player)->setEffect(EFFECT_SPEEDUP);
	AbstractAsteroidObject::die();
}