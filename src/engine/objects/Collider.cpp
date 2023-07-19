#include "../../../headers/engine/objects/Collider.hpp"

Collider::Collider(float radius, uint8_t collisionLayer, uint8_t collisionMask) {
    this->radius = radius;
    this->collisionLayer = collisionLayer;
    this->collisionMask = collisionMask;
}

uint8_t Collider::getCollisionLayer() {
    return collisionLayer;
}

uint8_t Collider::getCollisionMask() {
    return collisionMask;
}

float Collider::getRadius() {
    return radius;
}

void Collider::setParent(GameObject* object) {
    this->parent = object;
}

void Collider::setRadius(float _radius) {
    radius = _radius;
}

bool Collider::checkCollisionWith(Collider* collider2) {
    // Check masks
    bool maskCheck = (collisionMask & collider2->collisionLayer) != 0;
    if (!maskCheck) return false;
    // Get the origin of this collider
    glm::vec3 localOrigin = parent->transform.getPos();
    // Get the origin of the other collider
    glm::vec3 otherOrigin = collider2->parent->transform.getPos();
    // Check their relative distance
    float colliderDistance = glm::distance(localOrigin, otherOrigin);
    // Subtract radii
    float subtractedDistance = colliderDistance - radius - collider2->radius;
    //If the last value is negative, we have a collision!
    return subtractedDistance < 0;
}