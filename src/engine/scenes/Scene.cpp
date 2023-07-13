#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/Game.hpp"

Scene::Scene() {
    
}

void Scene::Draw(VkCommandBuffer commandBuffer, int currentImage) {
    //For each object in the scene, bind its pipeline and draw (WITHOUT rebinding the pipeline if it uses the same one)
    glm::mat4 cameraMatrix = camera.getCameraMatrix(aspectRatio);
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* object = activeObjects[i];
        object->Draw(commandBuffer, currentImage, this->activePipeline, cameraMatrix);
    }
}

void Scene::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}

void Scene::addObject(GameObject* object) {
    object->parentScene = this;
    activeObjects.push_back(object);
    if (dynamic_cast<ICollidable*>(this) != nullptr && object->collider != nullptr) {
        activeColliders[object->collider->getCollisionMask()].push_back(object->collider);
    }
}

void Scene::removeObject(GameObject* object) {
    object->parentScene = nullptr;
    int objectIdx = 0;
    for (int i = 0; i < activeObjects.size(); i++) {
        if (activeObjects[i] == object) {
            objectIdx = i;
            break;
        }
    }
    activeObjects.erase(activeObjects.begin() + objectIdx);
    removedObjects.push_back(object);
    ((Game*)proj)->recreateVulkanSwapChain();
}

void Scene::UpdateImpl(int currentimage) {
    glm::mat4 cameraMatrix = camera.getCameraMatrix(aspectRatio);
    Update();
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Update();
        activeObjects[i]->CommitUpdates(currentimage, cameraMatrix);
    }
    //Now check for collisions
    CheckCollisions();
}

void Scene::CheckCollisions() {
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* objectA = activeObjects[i];
        ICollidable* collidableA = dynamic_cast<ICollidable*>(objectA);
        for (int j = i + 1; j < activeObjects.size() && objectA->collider != nullptr && collidableA != nullptr; j++) {
            GameObject* objectB = activeObjects[j];
            ICollidable* collidableB = dynamic_cast<ICollidable*>(objectB);
            bool collision = (objectB->collider != nullptr) && (collidableB != nullptr) && objectA->collider->checkCollisionWith(objectB->collider);
            if (collision) {
                collidableA->OnCollisionWith(objectB);
                collidableB->OnCollisionWith(objectA);
            }
        }
    }
}

void Scene::CompileObjects() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->compile(proj, &gubos);
    }
}

void Scene::CleanupImpl() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Cleanup();
    }
    //Destroy objects that have been removed until now
    for (int i = 0; i < removedObjects.size(); i++) {
        removedObjects[i]->Cleanup();
        removedObjects[i]->Destroy();
    }
    removedObjects = {};
    Cleanup();
}

void Scene::DestroyImpl() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Destroy();
        delete activeObjects.at(i);
    }
    Destroy();
    activeObjects = {};
    for (int i = 0; i < removedObjects.size(); i++) {
        removedObjects[i]->Cleanup();
        removedObjects[i]->Destroy();
        delete removedObjects.at(i);
    }
    removedObjects = {};
}

Scene::~Scene() {
    //Everything deallocated on Destroy
}