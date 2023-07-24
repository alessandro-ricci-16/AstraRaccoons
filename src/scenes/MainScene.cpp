#include "../../headers/scenes/MainScene.hpp"
#include "../../headers/objects/SpaceshipObject.hpp"
#include "../../headers/objects/SkyBoxObject.hpp"
#include "../../headers/engine/base/Random.hpp"
#include <headers/objects/TestCubeObject.hpp>
#include <headers/engine/base/Time.hpp>
#include <headers/objects/MaterialTestSphere.hpp>
#include <headers/objects/asteroids/AbstractAsteroidObject.hpp>

#define ASTEROIDFACTORY_IMPLEMETATION
#include <headers/objects/asteroids/AsteroidFactory.hpp>
#include <headers/objects/asteroids/KillerPietrino.hpp>

void MainScene::Instantiate() {
	//Load & compile a test model
	SpaceshipObject* object = new SpaceshipObject();
	object->Instantiate();
	addObject(object);
	player = object;
	//Set up the camera
	camera = new Camera(glm::radians(90.0f), 0.1f, 300.0f, aspectRatio);
	camera->setTarget(&(object->transform));
	camera->setTargetDistance(vec3(0, 1.8f, 5.1f));
	camera->setTargetYOffset(7.0f);
	// Load skybox
	SkyBoxObject* skybox = new SkyBoxObject(camera);
	skybox->Instantiate();
	addObject(skybox);
	//Set up asteroid spawning
	spawnTimer = 0.0f;
	spawnDeltaTime = initialSpawnDeltaTime;
	// Set up lights
	gubos.pointLightPosition = glm::vec3(.5f, 0, 1.0f);
	gubos.pointLightColor = glm::vec4(0);
	gubos.pointLightDecayFactor = 2; // Inverse-linear
	gubos.pointLightTargetDistance = 2;
	gubos.directionalLightDirection = glm::vec3(-0.825236f, 0.462815f, -0.323709f);
	gubos.directionalLightColor = glm::vec4(1.0f);
	gubos.spotlightPosition = glm::vec3(0, 0, 1.2f);
	gubos.spotlightDirection = glm::vec3(0, 0, -1);
	gubos.spotlightColor = glm::vec4(1.0f);
	gubos.spotlightDecayFactor = 2;  // Inverse-linear
	gubos.spotlightTargetDistance = 35;
	gubos.spotlightCosIn = 0.95f;
	gubos.spotlightCosOut = 0.92f;
	gubos.eyePos = camera->getCameraPosition();
	// Spawn initial asteroids
	for (int i = 0; i < initialAsteroids; i++)
		AsteroidFactory::spawnRandomAsteroid(this, player);
}

void MainScene::Update() {
	//Update scene-level objects - NOT GameObjects, they are automatically managed by the scene
	//Rotate the directional light over time
	//gubos.directionalLightDirection = glm::rotate(glm::mat4(1), Time::getDeltaT() * 3, glm::vec3(0, 1, 0)) * glm::vec4(gubos.directionalLightDirection , 1);
	//Rotate the point light
	//gubos.pointLightPosition = glm::rotate(glm::mat4(1), Time::getDeltaT() * 3, glm::vec3(0, 0, 1)) * glm::vec4(gubos.pointLightPosition, 1);
	//Move the spot light
	//gubos.spotlightPosition = glm::vec3(9.f * sin(time), 0, -2);
	glm::vec3 playerUz = -player->transform.uz();
	gubos.spotlightPosition = player->transform.getPos() + playerUz * 21.65f * player->transform.getScale().z;
	gubos.spotlightDirection = playerUz;

	spawnTimer += Time::getFixedDeltaT();
	if (restartFlag) {
		restartFlag = false;
		// Add initial asteroids
		for (int i = 0; i < initialAsteroids; i++) {
			AsteroidFactory::spawnRandomAsteroid(this, player);
		}
		spawnTimer = 0.0f;
	}
	//Spawn new asteroids over time
	if (spawnTimer >= spawnDeltaTime && visibleAsteroids < maxVisibleAsteroids) {
		AsteroidFactory::spawnRandomAsteroid(this, player);
		spawnTimer = 0.0f;
	}
}

void MainScene::Destroy() {
	//Perform destruction of local objects not covered under the standard scene lifecycle
}

void MainScene::Cleanup() {
	//Perform cleanup of local objects not covered under the standard scene lifecycle
}

void MainScene::addObject(GameObject* object) {
	Scene::addObject(object);

	if (dynamic_cast<AbstractAsteroidObject*>(object) != nullptr) {
		visibleAsteroids += 1;
	}
}

void MainScene::removeObject(GameObject* object) {
	Scene::removeObject(object);

	if (dynamic_cast<AbstractAsteroidObject*>(object) != nullptr) {
		visibleAsteroids -= 1;
		spawnDeltaTime = max(spawnDeltaTime - loweringPerAsteroid, minSpawnRate);
	}
}

void MainScene::WillDisappear() {
	//Cleanup the scene & prepare for a scene reswitch
	//Clear all but the first 2 objects (spaceship & skybox)
	SpaceshipObject* spaceship;
	for (GameObject* activeObject : activeObjects) {
		SpaceshipObject* sp = dynamic_cast<SpaceshipObject*>(activeObject);
		SkyBoxObject* sk = dynamic_cast<SkyBoxObject*>(activeObject);
		if (sp == nullptr && sk == nullptr) {
			removeObject(activeObject);
		}
		else if (sp != nullptr) {
			spaceship = sp;
		}
	}
	restartFlag = true;
	camera->reset();
	if (gubos.pointLightColor.x != 0.0f) {
		gubos.pointLightColor = glm::vec4(0);
		AsteroidFactory::registerSpecialAsteroid<KillerPietrino>(true);
	}

	Score::resetScore();
	//Set up asteroid spawning
	spawnTimer = 0.0f;
	spawnDeltaTime = initialSpawnDeltaTime;
	//applyObjectModifications();
}