#include "../../headers/scenes/TestScene.hpp"
#include "../../headers/objects/SpaceshipObject.hpp"
#include "../../headers/objects/SkyBoxObject.hpp"
#include <headers/objects/TestCubeObject.hpp>
#include <headers/objects/asteroids/SimpleAsteroidObject.hpp>
#include <headers/engine/base/Time.hpp>

void TestScene::Instantiate() {
    //Load & compile a test model
    SpaceshipObject* object = new SpaceshipObject();
    object->Instantiate();
    addObject(object);
    player = object;
    //Set up the camera
    camera = Camera(glm::radians(90.0f), 0.1f, 100.0f, aspectRatio);
    camera.setTarget(&(object->transform));
    camera.setTargetDistance(vec3(0, 6, 17));
    // Load skybox
    SkyBoxObject* skybox = new SkyBoxObject(camera);
    skybox->Instantiate();
    addObject(skybox);
    //Set up asteroid spawning
    lastSpawnTime = 0;
    spawnDeltaTime = 1.5; //Seconds
    //Set up lights
    gubos.pointLightPosition = glm::vec3(.5f, 0, 1.0f);
    gubos.pointLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.pointLightDecayFactor = 2; // Inverse-linear
    gubos.pointLightTargetDistance = 2;
    gubos.directionalLightDirection = glm::vec3(0, 0, 1);
    gubos.directionalLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.spotlightPosition = glm::vec3(0, 0, 1.2f);
    gubos.spotlightDirection = glm::vec3(0, 0, -1);
    gubos.spotlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.spotlightDecayFactor = 2;  // Inverse-linear
    gubos.spotlightTargetDistance = 1.5f;
    gubos.spotlightCosIn = 0.92f;
    gubos.spotlightCosOut = 0.89f;
    gubos.eyePos = camera.getCameraPosition();
}

void TestScene::Update() {
    //Update scene-level objects - NOT GameObjects, they are automatically managed by the scene
    //Rotate the directional light over time
    //gubos.directionalLightDirection = glm::rotate(glm::mat4(1), Time::getDeltaT() * 3, glm::vec3(0, 1, 0)) * glm::vec4(gubos.directionalLightDirection , 1);
    //Rotate the point light
    gubos.pointLightPosition = glm::rotate(glm::mat4(1), Time::getDeltaT() * 3, glm::vec3(0, 0, 1)) * glm::vec4(gubos.pointLightPosition, 1);
    //Move the spot light
    //gubos.spotlightPosition = glm::vec3(9.f * sin(time), 0, -2);

    //Spawn new asteroids over time
    if (time - lastSpawnTime >= spawnDeltaTime) {
        SimpleAsteroidObject* asteroid = new SimpleAsteroidObject(&(player->transform));
        asteroid->Instantiate();
        addObject(asteroid);
        lastSpawnTime = time;
    }
    time += Time::getDeltaT();
}

void TestScene::Destroy() {
    //Perform destruction of local objects not covered under the standard scene lifecycle
}

void TestScene::Cleanup() {
    //Perform cleanup of local objects not covered under the standard scene lifecycle
}