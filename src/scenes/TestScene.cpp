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
    
}

void TestScene::Destroy() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects.at(i)->Destroy();
    }
}