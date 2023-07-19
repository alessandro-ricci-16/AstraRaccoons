#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/Game.hpp"

Scene::Scene(float* ar) {
    aspectRatio = ar;
    modifiedActiveObjects = false;
    isUpdatingScene = false;
}

void Scene::Draw(VkCommandBuffer commandBuffer, int currentImage) {
    //For each object in the scene, bind its pipeline and draw (WITHOUT rebinding the pipeline if it uses the same one)
    this->activePipeline = nullptr;
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* object = activeObjects[i];
        object->Draw(commandBuffer, currentImage, this->activePipeline);
    }
}

void Scene::addObject(GameObject* object) {
    addedObjects.push_back(object);
}

void Scene::removeObject(GameObject* object) {
    removedObjects.push_back(object);
}

void Scene::applyObjectModifications() {
    // adding new objects
    for (int i = 0; i < addedObjects.size(); i++) {
        GameObject* object = addedObjects[i];
        object->parentScene = this;
        activeObjects.push_back(object);
        modifiedActiveObjects = true;
        if (dynamic_cast<ICollidable*>(object) != nullptr && object->collider != nullptr) {
            activeColliders[object->collider->getCollisionMask()].push_back(object->collider);
        }
    }
    addedObjects.clear();
    // removing objects
    for (int i = 0; i < removedObjects.size(); i++) {
        GameObject* objectToRemove = removedObjects[i];
        objectToRemove->parentScene = nullptr;
        // remove collider
        if (objectToRemove->collider != nullptr) {
            auto colliders = activeColliders[objectToRemove->collider->getCollisionMask()];
            for (int i = 0; i < colliders.size(); i++) {
                if (colliders[i] == objectToRemove->collider) {
                    colliders.erase(colliders.begin() + i);
                    break;
                }
            }
        }
        // remove from active objects
        for (int i = 0; i < activeObjects.size(); i++) {
            if (activeObjects[i] == objectToRemove) {
                modifiedActiveObjects = true;
                activeObjects.erase(activeObjects.begin() + i);
                break;
            }
        }

        objectToRemove->Cleanup();
        objectToRemove->Destroy();
        delete objectToRemove;
    }
    removedObjects.clear();
}

int Scene::totalTextureCount() {
    int texCount = 0;
    for (int i = 0; i < activeObjects.size(); i++) {
        texCount += activeObjects[i]->textureCount();
    }
    return texCount;
}

int Scene::totalUniformsCount() {
    int uniformsCount = 0;
    for (int i = 0; i < activeObjects.size(); i++) {
        uniformsCount += activeObjects[i]->uniformsCount();
    }
    return uniformsCount;
}

void Scene::UpdateImpl(int currentimage) {
    isUpdatingScene = true;
    Update();
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Update();
    }
    isUpdatingScene = false;
    glm::mat4 cameraMatrix = camera->getCameraMatrix();
    gubos.eyePos = camera->getCameraPosition();
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->CommitUpdates(currentimage, cameraMatrix);
    }
    //Now check for collisions
    CheckCollisions();
    //Apply object removals
    applyObjectModifications();
    //Check if we need to recreate the swapchain
    if (modifiedActiveObjects) {
        ((Game*)proj)->recreateVulkanSwapChain(true);
    }
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

void Scene::CompileObjects(bool addedOnly) {
    std::vector<GameObject*> objectsToCompile = activeObjects;
    for (int i = 0; i < objectsToCompile.size(); i++) {
        objectsToCompile[i]->compile(proj, &gubos);
    }
    modifiedActiveObjects = false;
}

void Scene::CleanupImpl() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Cleanup();
    }
    Cleanup();
}

void Scene::DestroyImpl() {
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Destroy();
        delete activeObjects.at(i);
    }
    Destroy();
    activeObjects.clear();
}

Scene::~Scene() {
    //Everything deallocated on Destroy
}