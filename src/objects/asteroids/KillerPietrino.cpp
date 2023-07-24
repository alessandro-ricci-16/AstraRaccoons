#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/SpaceshipObject.hpp"
#include "../../../headers/objects/asteroids/KillerPietrino.hpp"

#include <headers/scenes/AvailableScenes.hpp>

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"

static const bool KillerPietrinoRegistered =
(AsteroidFactory::registerSpecialAsteroid<KillerPietrino>(true), true);

void KillerPietrino::Instantiate() {
	AbstractAsteroidObject::Instantiate();

	model.addUniformData(&additionalUniforms, sizeof(KillerPietrinoUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	// special pietrino color
	additionalUniforms.time = 0.0f;

	model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv",
		"shaders/asteroids/KillerPietrino_Frag.spv");
	model.addTexture("textures/asteroids/killer/Emission.jpg");

	scaleToUpdate = 20.f;
}

void KillerPietrino::Update() {
	AbstractAsteroidObject::Update();
	additionalUniforms.time += Time::getDeltaT();

	//Update GUBOs for the point light
	parentScene->gubos.pointLightColor = glm::vec4(1.f, 0.44f, 0.22f, 1.f) * 8.f;
	parentScene->gubos.pointLightDecayFactor = 1.5f;
	parentScene->gubos.pointLightTargetDistance = 40.f;
	parentScene->gubos.pointLightPosition = transform.getPos();
}

std::string KillerPietrino::getModelName() { return "models/Sphere.gltf"; }

void KillerPietrino::receiveDamage(float damage) {
	//No damage here!
}

void KillerPietrino::die() {
	//Do not despawn
}
