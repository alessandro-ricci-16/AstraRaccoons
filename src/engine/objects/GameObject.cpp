#include "../../../headers/engine/objects/GameObject.hpp"

GameObject::GameObject() {
    transform = Transform::identity();
}

void GameObject::addChild(GameObject* child) {
    child->parentObject = this;
    children.push_back(child);
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