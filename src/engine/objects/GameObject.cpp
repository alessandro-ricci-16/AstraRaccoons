#include "../../../headers/engine/objects/GameObject.hpp"
#include "../../../headers/engine/objects/Collider.hpp"

GameObject::GameObject() {
    transform = Transform::identity();
}

void GameObject::addChild(GameObject* child) {
    child->parentObject = this;
    children.push_back(child);
}

void GameObject::addCollider(glm::vec3 origin, float radius, uint8_t collisionLayer, uint8_t collisionMask) {
    if (dynamic_cast<ICollidable*>(this) == nullptr) {
        std::cout << "WARNING: Trying to set a collider to object " << this << " which does not conform to ICollidable! For safety, a collider will not be added to this object.\n";
    } else {
        colliders.push_back(new Collider(this, radius, collisionLayer, collisionMask, origin));
    }
}

int GameObject::textureCount() {
    int myTexCount = 0;
    for (int i = 0; i < children.size(); i++) {
        myTexCount += children[i]->textureCount();
    }
    return myTexCount;
}

int GameObject::uniformsCount() {
    int myUniformCount = 0;
    for (int i = 0; i < children.size(); i++) {
        myUniformCount += children[i]->uniformsCount();
    }
    return myUniformCount;
}

void GameObject::Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) {
    // Don't do anything since this object has no mesh!
    // Recursively draw all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Draw(commandBuffer, currentImage, activePipeline);
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

mat4 GameObject::getFullMatrix() {
    if (parentObject == nullptr) {
        return transform.getMatrix();
    } else {
        return parentObject->getFullMatrix() * transform.getMatrix();
    }
}

void GameObject::UpdateImpl() {
    Update();
    for (int i = 0; i < children.size(); i++) {
        children[i]->UpdateImpl();
    }
}

bool GameObject::getDisplaysAlwaysOnTop() {
    return shouldDisplayAlwaysOnTop;
}

GameObject::~GameObject() {
    for (int i = 0; i < children.size(); i++) {
        delete children.at(i);
    }
    children.clear();
    for (int i = 0; i < colliders.size(); i++) {
        delete colliders[i];
    }
    colliders.clear();
}