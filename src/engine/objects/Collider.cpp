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

void Collider::setParent(GameObject* object) {
    this->parent = object;
}

bool Collider::checkCollisionWith(Collider* collider2) {
    //Check masks
    bool maskCheck = (collisionMask & collider2->collisionLayer) != 0;
    //Get the origin of this collider
    glm::vec3 localOrigin = (parent->transform.getMatrix()) * glm::vec4(0, 0, 0, 1);
    //Get the origin of the other collider
    glm::vec3 otherOrigin = (collider2->parent->transform.getMatrix()) * glm::vec4(0, 0, 0, 1);
    //Check their relative distance
    float colliderDistance = glm::distance(localOrigin, otherOrigin);
    //Subtract radii
    float subtractedDistance = colliderDistance - radius - collider2->radius;
    //If the last value is negative, we have a collision!
    return subtractedDistance < 0 && maskCheck;
}