#include <headers/engine/base/Time.hpp>
#include <headers/objects/SkyBoxObject.hpp>
#include <headers/scenes/IntroScene.hpp>
#include <headers/engine/objects/OrbitingCamera.hpp>
#include <headers/objects/FaderPlaneObject.hpp>
#include <headers/engine/base/Inputs.hpp>
#include <headers/Game.hpp>
#include <headers/scenes/AvailableScenes.hpp>

void IntroScene::Instantiate() {
    // Load & compile a test model
    lookAtTarget = new Transform();
    lookAtTarget->TranslateTo(glm::vec3(0, 0, -1));
    // Set up the camera
    camera = new OrbitingCamera(glm::radians(90.0f), 0.1f, 100.0f, aspectRatio);
    camera->setTarget(lookAtTarget);
    camera->setTargetDistance(vec3(0, 0, 1));
    // Load skybox
    SkyBoxObject* skybox = new SkyBoxObject(camera);
    skybox->Instantiate();
    addObject(skybox);
    // Fader
    FaderPlaneObject* fader = new FaderPlaneObject();
    fader->Instantiate();
    addObject(fader);

    gubos.eyePos = camera->getCameraPosition();
}

void IntroScene::Update() {
    // Update scene-level objects - NOT GameObjects, they are automatically
    // managed by the scene Rotate the directional light over time
    // Rotate the look at target to rotate the camera slowly over time
    lookAtTarget->TranslateTo(glm::rotate(glm::mat4(1), (float)fmod(0.35f * time, 360), glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

    time += Time::getDeltaT();

    if (Inputs::isKeyPressed(GLFW_KEY_SPACE)) {
        ((Game*)proj)->switchToScene(AR_SCENE_MAIN);
    }
}

void IntroScene::Destroy() {
    // Perform destruction of local objects not covered under the standard scene
    // lifecycle
}

void IntroScene::Cleanup() {
    // Perform cleanup of local objects not covered under the standard scene
    // lifecycle
}