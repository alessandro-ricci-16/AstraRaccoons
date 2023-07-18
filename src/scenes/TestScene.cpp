#include <headers/engine/base/Time.hpp>
#include <headers/objects/SkyBoxObject.hpp>
#include <headers/scenes/TestScene.hpp>
#include <headers/engine/objects/OrbitingCamera.hpp>
#include <headers/objects/TestCubeObject.hpp>
#include <headers/engine/base/Inputs.hpp>
#include <headers/Game.hpp>
#include <headers/scenes/AvailableScenes.hpp>

void TestScene::Instantiate() {
    // Load & compile a test model
    TestCubeObject* object = new TestCubeObject();
    object->Instantiate();
    addObject(object);
    lookAtTarget = new Transform(glm::mat4(1), glm::mat4(1), glm::mat4(1));
    // Set up the camera
    camera = new OrbitingCamera(glm::radians(90.0f), 0.1f, 100.0f, aspectRatio);
    camera->setTarget(lookAtTarget);
    camera->setTargetDistance(vec3(0, 0, 2));

    gubos.pointLightPosition = glm::vec3(.5f, 0, 1.0f);
    gubos.pointLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.pointLightDecayFactor = 2;  // Inverse-linear
    gubos.pointLightTargetDistance = 2;
    gubos.directionalLightDirection = glm::vec3(0, 0, 1);
    gubos.directionalLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.spotlightPosition = glm::vec3(0, 0, 1.7);
    gubos.spotlightDirection = glm::vec3(0, 0, -1);
    gubos.spotlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.spotlightDecayFactor = 2;  // Inverse-linear
    gubos.spotlightTargetDistance = 1.5f;
    gubos.spotlightCosIn = 0.92f;
    gubos.spotlightCosOut = 0.89f;
    gubos.eyePos = camera->getCameraPosition();
}

void TestScene::Update() {
    // Update scene-level objects - NOT GameObjects, they are automatically
    // managed by the scene Rotate the directional light over time
    // Rotate the look at target to rotate the camera slowly over time
    time += Time::getDeltaT();
}

void TestScene::Destroy() {
    // Perform destruction of local objects not covered under the standard scene
    // lifecycle
}

void TestScene::Cleanup() {
    // Perform cleanup of local objects not covered under the standard scene
    // lifecycle
}