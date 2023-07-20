#include <headers/Game.hpp>
#include <headers/engine/base/Inputs.hpp>
#include <headers/engine/base/Time.hpp>
#include <headers/engine/objects/OrbitingCamera.hpp>
#include <headers/objects/FaderPlaneObject.hpp>
#include <headers/objects/SkyBoxObject.hpp>
#include <headers/objects/TextMaker.hpp>
#include <headers/scenes/AvailableScenes.hpp>
#include <headers/scenes/GameOverScene.hpp>

void GameOverScene::Instantiate() {
    // Load & compile a test model
    lookAtTarget = new Transform();
    lookAtTarget->TranslateTo(glm::vec3(0, 0, -1));
    // Set up the camera
    camera = new OrbitingCamera(glm::radians(90.0f), 0.1f, 100.0f, aspectRatio);
    camera->setTarget(lookAtTarget);
    camera->setTargetDistance(vec3(0));
    // Load skybox
    SkyBoxObject* skybox = new SkyBoxObject(camera);
    skybox->Instantiate();
    addObject(skybox);
    // Fader
    FaderPlaneObject* fader = new FaderPlaneObject();
    fader->Instantiate();
    addObject(fader);
    // Text
    TextMaker* txt = new TextMaker();
    txt->Instantiate();
    addObject(txt);

    gubos.eyePos = camera->getCameraPosition();
}

void GameOverScene::Update() {
    // Update scene-level objects - NOT GameObjects, they are automatically
    // managed by the scene Rotate the directional light over time

    if (Inputs::isKeyPressed(GLFW_KEY_SPACE)) {
        ((Game*)proj)->switchToScene(AR_SCENE_MAIN);
    }
}

void GameOverScene::Destroy() {
    // Perform destruction of local objects not covered under the standard scene
    // lifecycle
}

void GameOverScene::Cleanup() {
    // Perform cleanup of local objects not covered under the standard scene
    // lifecycle
}

void GameOverScene::WillDisappear() {
    // Cleanup the scene & prepare for a scene reswitch
}