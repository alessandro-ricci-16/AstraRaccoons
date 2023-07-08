#include "../../headers/scenes/TestScene.hpp"
#include "../../headers/objects/TestCubeObject.hpp"

void TestScene::Instantiate() {
    //Load & compile a test model
    TestCubeObject* object = new TestCubeObject();
    object->Instantiate();
    activeObjects.push_back(object);
    object->compile(proj, nullptr);
    //Set up the camera
    camera = Camera(glm::radians(90.0f), 0.1f, 100.0f);
    camera.setTarget(&(object->transform));
    camera.setTargetDistance(10);
}

void TestScene::Update() {
    //Update scene-level objects - NOT GameObjects, they are automatically managed by the scene
}

void TestScene::Destroy() {
    //Perform destruction of local objects not covered under the standard scene lifecycle
}