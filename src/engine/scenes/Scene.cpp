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
	for (GameObject* object: activeObjects) {
		object->Draw(commandBuffer, currentImage, this->activePipeline);
	}
}

void Scene::addObject(GameObject* object) {
	addedObjects.insert(object);
}

void Scene::removeObject(GameObject* object) {
	removedObjects.insert(object);
}

void Scene::applyObjectModifications() {
	// adding new objects
	for (GameObject* objectToAdd: addedObjects) {
		objectToAdd->parentScene = this;
		activeObjects.insert(objectToAdd);
		modifiedActiveObjects = true;
		for (int i = 0; i < objectToAdd->colliders.size(); i++) {
			activeColliders.insert(objectToAdd->colliders[i]);
		}
	}
	addedObjects.clear();
	// removing objects
	for (GameObject* objectToRemove: removedObjects) {
		objectToRemove->parentScene = nullptr;
		// remove collider
		if (!objectToRemove->colliders.empty()) {
			for (int j = 0; j < objectToRemove->colliders.size(); j++) {
				for (Collider* activeCollider: activeColliders) {
					if (activeCollider == objectToRemove->colliders[j]) {
						activeColliders.erase(activeCollider);
						break;
					}
				}
			}
		}
		// remove from active objects
		for (GameObject* activeObject: activeObjects) {
			if (activeObject == objectToRemove) {
				modifiedActiveObjects = true;

				activeObjects.erase(activeObject);
				objectToRemove->Cleanup();
				objectToRemove->Destroy();
				delete objectToRemove;
				break;
			}
		}
	}
	removedObjects.clear();
	// Check if we need to recreate the swapchain
	if (modifiedActiveObjects) {
		((Game*)proj)->recreateVulkanSwapChain(true);
	}
}

int Scene::totalTextureCount() {
	int texCount = 0;
	for (GameObject* activeObject: activeObjects) {
		texCount += activeObject->textureCount();
	}
	return texCount;
}

int Scene::totalUniformsCount() {
	int uniformsCount = 0;
	for (GameObject* activeObject: activeObjects) {
		uniformsCount += activeObject->uniformsCount();
	}
	return uniformsCount;
}

void Scene::UpdateImpl(int currentimage) {
	isUpdatingScene = true;
	Update();
	for (GameObject* activeObject: activeObjects) {
		activeObject->UpdateImpl();
	}
	isUpdatingScene = false;
	glm::mat4 cameraMatrix = camera->getCameraMatrix();
	gubos.eyePos = camera->getCameraPosition();
	for (GameObject* activeObject: activeObjects) {
		activeObject->CommitUpdates(currentimage, cameraMatrix);
	}
	//Now check for collisions
	CheckCollisions();
	//If scene switch was requested and granted, then call WillDisappear
	if (sceneSwitchRequested) {
		sceneSwitchRequested = false;
		WillDisappear();
	}
	//Apply object modifications happened mid-frame
	applyObjectModifications();
}

void Scene::CheckCollisions() {
	std::vector<Collider*> activeCollidersList(activeColliders.begin(), activeColliders.end());
	int nColliders = (int)activeCollidersList.size();
	if (nColliders < 2) return;

	for (int i = 0; i < nColliders; i++) {
		Collider* colliderA = activeCollidersList[i];
		for (int j = i + 1; j < nColliders && colliderA != nullptr; j++) {
			Collider* colliderB = activeCollidersList[j];
			bool collision = (colliderB != nullptr) && colliderA->checkCollisionWith(colliderB);
			if (collision) {
				GameObject* objectA = colliderA->getParent();
				GameObject* objectB = colliderB->getParent();
				if (objectA && objectB) { // both not null
					ICollidable* collidableA = (dynamic_cast<ICollidable*>(objectA));
					ICollidable* collidableB = (dynamic_cast<ICollidable*>(objectB));
					colliderA->compensateCompenetrations(colliderB);
					collidableA->OnCollisionWith(colliderB);
					collidableB->OnCollisionWith(colliderA);
				}
			}
		}
	}
}

void Scene::CompileObjects(bool addedOnly) {
	for (GameObject* activeObject: activeObjects) {
		activeObject->compile(proj, &gubos);
	}
	modifiedActiveObjects = false;
}

void Scene::requestSceneSwitch(int newScene) {
	sceneSwitchRequested = proj->requestSwitchToScene(newScene);
}

void Scene::CleanupImpl() {
	for (GameObject* activeObject: activeObjects) {
		activeObject->Cleanup();
	}
	Cleanup();
}

void Scene::DestroyImpl() {
	for (GameObject* activeObject: activeObjects) {
		activeObject->Destroy();
		delete activeObject;
	}
	Destroy();
	activeObjects.clear();
}

Scene::~Scene() {
	//Everything deallocated on Destroy
}
