#include "../../headers/scenes/TestScene.hpp"
#include "../../headers/objects/TestCubeObject.hpp"

void TestScene::Instantiate() {
    //Load & compile a test model
    TestCubeObject* object = new TestCubeObject();
    object->Instantiate();
    addObject(object);
    TestCubeObject* object2 = new TestCubeObject();
    object2->Instantiate();
    object2->transform.TranslateTo(glm::vec3(3, 0, 0));
    //object2->moves = true;
    addObject(object2);
    TestCubeObject* object3 = new TestCubeObject();
    object3->Instantiate();
    object3->transform.TranslateTo(glm::vec3(0, 2, 0));
    addObject(object3);
    TestCubeObject* object4 = new TestCubeObject();
    object4->Instantiate();
    object4->transform.TranslateTo(glm::vec3(0, 4, 0));
    addObject(object4);
    //Set up the camera
    camera = Camera(glm::radians(90.0f), 0.1f, 100.0f);
    camera.setTarget(&(object->transform));
    camera.setTargetDistance(10);
    //Set up lights
    gubos.pointLightPosition = glm::vec3(-1.0f, 0, 0.0f);
    gubos.pointLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubos.eyePos = camera.getCameraPosition(aspectRatio);
}

void TestScene::Update() {
    //Update scene-level objects - NOT GameObjects, they are automatically managed by the scene
    gubos.eyePos = camera.getCameraPosition(aspectRatio);
}

void TestScene::Destroy() {
    //Perform destruction of local objects not covered under the standard scene lifecycle
}

void TestScene::Cleanup() {
    //Perform cleanup of local objects not covered under the standard scene lifecycle
}