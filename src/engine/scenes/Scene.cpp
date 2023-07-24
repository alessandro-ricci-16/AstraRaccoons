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
	for (ActiveObjectElement* element: activeObjects) {
		element->object->Draw(commandBuffer, currentImage, this->activePipeline);
	}
}

void Scene::addObject(GameObject* object) {
	addedObjects.insert(object);
}

void Scene::removeObject(GameObject* object) {
	removedObjects.insert(object);
}

void Scene::applyObjectModifications() {
	//First off, remove from addedObjects ALL objects that are in removedObjects: objects created during this frame should not be added/removed, but just deleted!
	std::vector<GameObject*> ignoredObjects = {};
	for (GameObject* objectToRemove : removedObjects) {
		// remove from active objects
		for (GameObject* addedObject : addedObjects) {
			if (addedObject == objectToRemove) {
				addedObjects.erase(addedObject);
				ignoredObjects.push_back(objectToRemove);
				delete objectToRemove;
				break;
			}
		}
	}
	for (int i = 0; i < ignoredObjects.size(); i++) {
		removedObjects.erase(ignoredObjects[i]);
	}
	// adding new objects
	for (GameObject* objectToAdd: addedObjects) {
		objectToAdd->parentScene = this;
		ActiveObjectElement* element = new ActiveObjectElement{objectToAdd, newZIndex};
		newZIndex += 1;
		activeObjects.insert(element);
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
		for (ActiveObjectElement* activeObject: activeObjects) {
			if (activeObject->object == objectToRemove) {
				modifiedActiveObjects = true;

				activeObjects.erase(activeObject);
				objectToRemove->Cleanup();
				objectToRemove->Destroy();
				delete objectToRemove;
				delete activeObject;
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
	for (ActiveObjectElement* activeObject: activeObjects) {
		texCount += activeObject->object->textureCount();
	}
	return texCount;
}

int Scene::totalUniformsCount() {
	int uniformsCount = 0;
	for (ActiveObjectElement* activeObject: activeObjects) {
		uniformsCount += activeObject->object->uniformsCount();
	}
	return uniformsCount;
}

void Scene::UpdateImpl(int currentimage) {
	isUpdatingScene = true;
	Update();
	for (ActiveObjectElement* activeObject: activeObjects) {
		activeObject->object->UpdateImpl();
	}
	isUpdatingScene = false;
	glm::mat4 cameraMatrix = camera->getCameraMatrix();
	gubos.eyePos = camera->getCameraPosition();
	for (ActiveObjectElement* activeObject: activeObjects) {
		activeObject->object->CommitUpdates(currentimage, cameraMatrix);
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
	for (ActiveObjectElement* activeObject: activeObjects) {
		activeObject->object->compile(proj, &gubos);
	}
	modifiedActiveObjects = false;
}

void Scene::requestSceneSwitch(int newScene) {
	sceneSwitchRequested = proj->requestSwitchToScene(newScene);
}

void Scene::CleanupImpl() {
	for (ActiveObjectElement* activeObject: activeObjects) {
		activeObject->object->Cleanup();
	}
	Cleanup();
}

void Scene::DestroyImpl() {
	for (ActiveObjectElement* activeObject: activeObjects) {
		activeObject->object->Destroy();
		delete activeObject->object;
		delete activeObject;
	}
	Destroy();
	activeObjects.clear();
}

Scene::~Scene() {
	//Everything deallocated on Destroy
}

void Scene::windowResize() {
	text = txt->getText();
	x = txt->getXCen();
	y = txt->getYCen();
	removeObject(txt);
	txt = new TextMaker(text.c_str(), x, y);
	txt->SetDimensions(proj->getWidth(), proj->getHeight());
	txt->Instantiate();
	addObject(txt);
}