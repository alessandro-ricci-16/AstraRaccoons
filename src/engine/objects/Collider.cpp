#include "../../../headers/engine/objects/Collider.hpp"

Collider::Collider(GameObject* _parent, float _radius, uint8_t _collisionLayer, uint8_t _collisionMask, glm::vec3 _origin) {
	parent = _parent;
	radius = _radius;
	collisionLayer = _collisionLayer;
	collisionMask = _collisionMask;
	origin = _origin;
}

uint8_t Collider::getCollisionLayer() {
	return collisionLayer;
}

uint8_t Collider::getCollisionMask() {
	return collisionMask;
}

GameObject* Collider::getParent() {
	return parent;
}

float Collider::getRadius() {
	return radius;
}

void Collider::setRadius(float _radius) {
	radius = _radius;
}

bool Collider::checkCollisionWith(Collider* collider2) {
	// Check masks
	bool maskCheck = (collisionMask & collider2->collisionLayer) != 0;
	if (!maskCheck) return false;
	// Get the origin of this collider
	Transform parentTransform = getParent()->transform;
	Transform otherTransform = collider2->getParent()->transform;
    glm::vec3 localOrigin = parentTransform.getWorldPositionOfLocal(origin);
    // Get the origin of the other collider
	glm::vec3 otherOrigin = otherTransform.getWorldPositionOfLocal(collider2->origin);
	// Check their relative distance
	float colliderDistance = glm::distance(localOrigin, otherOrigin);
	// Subtract radii
	float subtractedDistance = colliderDistance - radius - collider2->radius;
	//If the last value is negative, we have a collision!
	return subtractedDistance < 0;
}

void Collider::compensateCompenetrations(Collider* collider2) {
	// Check masks
	bool maskCheck = (collisionMask & collider2->collisionLayer) != 0;
	if (!maskCheck) return;
	// Get the origin of this collider
	Transform parentTransform = getParent()->transform;
	Transform otherTransform = collider2->getParent()->transform;
    glm::vec3 localOrigin = parentTransform.getWorldPositionOfLocal(origin);
    // Get the origin of the other collider
	glm::vec3 otherOrigin = otherTransform.getWorldPositionOfLocal(collider2->origin);
	// Check their relative distance
	float colliderDistance = glm::distance(localOrigin, otherOrigin);
	// Subtract radii
	float subtractedDistance = colliderDistance - radius - collider2->radius;
	
	if (subtractedDistance < 0) {
		glm::vec3 translationVector = (localOrigin - otherOrigin) * -subtractedDistance * 0.5f / colliderDistance;
		getParent()->transform.TranslateBy(translationVector);
		collider2->getParent()->transform.TranslateBy(-translationVector);
	}
}