#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/Game.hpp"

Scene::Scene(float* ar) {
    aspectRatio = ar;
    modifiedActiveObjects = false;
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
    object->parentScene = this;
    activeObjects.push_back(object);
    addedObjects.push_back(object);
    if (dynamic_cast<ICollidable*>(this) != nullptr && object->collider != nullptr) {
        activeColliders[object->collider->getCollisionMask()].push_back(object->collider);
    }
    modifiedActiveObjects = true;
}

void Scene::removeObject(GameObject* object) {
    removedObjects.push_back(object);
    modifiedActiveObjects = true;
}

void Scene::applyObjectRemoval() {
    for (int i = 0; i < removedObjects.size(); i++) {
        GameObject* object = removedObjects[i];
        object->parentScene = nullptr;
        int objectIdx = 0;
        for (int i = 0; i < activeObjects.size(); i++) {
            if (activeObjects[i] == object) {
                objectIdx = i;
                break;
            }
        }
        activeObjects.erase(activeObjects.begin() + objectIdx);
        if (addedObjects.size() > 0) {
            objectIdx = 0;
            for (int i = 0; i < addedObjects.size(); i++) {
                if (addedObjects[i] == object) {
                    objectIdx = i;
                    break;
                }
            }
            addedObjects.erase(addedObjects.begin() + objectIdx);
        }
        object->Cleanup();
        object->Destroy();
        delete object;
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
    Update();
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->Update();
    }
    glm::mat4 cameraMatrix = camera->getCameraMatrix();
    gubos.eyePos = camera->getCameraPosition();
    for (int i = 0; i < activeObjects.size(); i++) {
        activeObjects[i]->CommitUpdates(currentimage, cameraMatrix);
    }
    //Now check for collisions
    CheckCollisions();
    //Apply object removals
    applyObjectRemoval();
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
    std::vector<GameObject*> objectsToCompile = addedOnly ? addedObjects : activeObjects;
    for (int i = 0; i < objectsToCompile.size(); i++) {
        objectsToCompile[i]->compile(proj, &gubos);
    }
    addedObjects.clear();
    modifiedActiveObjects = false;
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