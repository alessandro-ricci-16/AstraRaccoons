#include "../../../headers/engine/objects/GameObject.hpp"
#include "../../../headers/engine/objects/Collider.hpp"

GameObject::GameObject() {
    transform = Transform::identity();
}

void GameObject::addChild(GameObject* child) {
    child->parentObject = this;
    children.push_back(child);
}

void GameObject::setCollider(float radius, uint8_t collisionLayer, uint8_t collisionMask) {
    if (dynamic_cast<ICollidable*>(this) == nullptr) {
        std::cout << "WARNING: Trying to set a collider to object " << this << " which does not conform to ICollidable! For safety, a collider will not be added to this object.\n";
    } else {
        collider = new Collider(radius, collisionLayer, collisionMask);
        collider->setParent(this);
    }
}

void GameObject::Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix) {
    // Don't do anything since this object has no mesh!
    // Recursively draw all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Draw(commandBuffer, currentImage, activePipeline, cameraMatrix);
    }
}

void GameObject::CommitUpdates(int currentImage, glm::mat4 cameraMatrix) {
    // Don't do anything since this object has no mesh!
    // Recursively draw all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->CommitUpdates(currentImage, cameraMatrix);
    }
}

void GameObject::compile(BaseProject* proj, GlobalUniforms* guboPtr) {
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->compile(proj, guboPtr);
    }
}

GameObject::~GameObject() {
    for (int i = 0; i < children.size(); i++) {
        delete children.at(i);
    }
    children.clear();
}