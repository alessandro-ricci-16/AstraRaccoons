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
		GameObject* objectToAdd = addedObjects[i];
		objectToAdd->parentScene = this;
		activeObjects.push_back(objectToAdd);
		modifiedActiveObjects = true;
		if (objectToAdd->collider != nullptr) {
			activeColliders.push_back(objectToAdd->collider);
		}
	}
	addedObjects.clear();
	// removing objects
	for (int i = 0; i < removedObjects.size(); i++) {
		GameObject* objectToRemove = removedObjects[i];
		objectToRemove->parentScene = nullptr;
		// remove collider
		if (objectToRemove->collider != nullptr) {
			for (int i = 0; i < activeColliders.size(); i++) {
				if (activeColliders[i] == objectToRemove->collider) {
					activeColliders.erase(activeColliders.begin() + i);
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
	// Check if we need to recreate the swapchain
	if (modifiedActiveObjects) {
		((Game*)proj)->recreateVulkanSwapChain(true);
	}
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
	//Switch scene if needed
	if (requestedSceneToSwitch >= 0) {
		proj->switchToScene(requestedSceneToSwitch);
		requestedSceneToSwitch = -1;
	}
}

void Scene::CheckCollisions() {
	int nColliders = activeColliders.size();
	if (nColliders < 2) return;

	for (int i = 0; i < nColliders; i++) {
		Collider* colliderA = activeColliders[i];
		for (int j = i + 1; j < nColliders && colliderA != nullptr; j++) {
			Collider* colliderB = activeColliders[j];
			bool collision = (colliderB != nullptr) && colliderA->checkCollisionWith(colliderB);
			if (collision) {
				GameObject* objectA = colliderA->getParent();
				GameObject* objectB = colliderB->getParent();
				if (objectA && objectB) { // both not null
					ICollidable* collidableA = (dynamic_cast<ICollidable*>(objectA));
					ICollidable* collidableB = (dynamic_cast<ICollidable*>(objectB));
					colliderA->compensateCompenetrations(colliderB);
					collidableA->OnCollisionWith(objectB);
					collidableB->OnCollisionWith(objectA);
				}
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

void Scene::requestSceneSwitch(int newScene) {
	requestedSceneToSwitch = newScene;
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