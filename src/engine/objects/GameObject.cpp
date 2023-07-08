#include "../../../headers/engine/objects/GameObject.hpp"

GameObject::GameObject() {
    transform = Transform::identity();
}

void GameObject::addChild(GameObject child) {
    child.parentObject = this;
    children.push_back(child);
}

void GameObject::Instantiate() {

}

void GameObject::Start() {
    
}

void GameObject::Update() {
    
}

void GameObject::Destroy() {
    
}