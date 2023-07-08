#include "../../../headers/engine/scenes/Scene.hpp"

Scene::Scene() {
    
}

void Scene::Draw(VkCommandBuffer commandBuffer, int currentImage) {
    //For each object in the scene, bind its pipeline and draw (WITHOUT rebinding the pipeline if it uses the same one)
    glm::mat4 cameraMatrix = camera.getCameraMatrix(aspectRatio);
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* object = activeObjects.at(i);
        object->Draw(commandBuffer, currentImage, this->activePipeline, cameraMatrix);
    }
}

void Scene::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Scene::addObject(GameObject* object) {
    activeObjects.push_back(object);
}

void Scene::UpdateImpl(int currentimage) {
    glm::mat4 cameraMatrix = camera.getCameraMatrix(aspectRatio);
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects.at(i)->Update();
        activeObjects.at(i)->CommitUpdates(currentimage, cameraMatrix);
    }
}