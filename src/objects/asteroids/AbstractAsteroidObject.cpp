#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/base/Time.hpp"
#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/AbstractAsteroidObject.hpp"
#include "../../../headers/objects/Pew.hpp"
#include "../../../headers/engine/base/Random.hpp"
#include <headers/objects/SpaceshipObject.hpp>

AbstractAsteroidObject::AbstractAsteroidObject(Transform* _playerTransform, float _initialScale) {
	transform = Transform::identity();
	scale = _initialScale;
	minScale = std::min(scale, 1.5f);
	scaleToUpdate = scale;
	transform.ScaleTo(glm::vec3(scale));
	playerTransform = _playerTransform;
}

void AbstractAsteroidObject::Instantiate() {
	// Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(AbstractAsteroidVertex), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AbstractAsteroidVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(AbstractAsteroidVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AbstractAsteroidVertex, norm), sizeof(glm::vec3), NORMAL);
	//Position the asteroid in a random point around the player at a random distance and with random velocities
	glm::vec3 playerPosition = transform.getPos();
	glm::vec3 randomDirection = glm::normalize(glm::vec3(Random::randomFloat(-1, 1), Random::randomFloat(-1, 1), Random::randomFloat(-1, 1)));
	float randomDistance = Random::randomFloat(80, 100);
	float randomVelocity = Random::randomFloat(1.5f, 8);
	float randomAngularVelocity = Random::randomFloat(2, 10);
	glm::vec3 randomRotationAxis = glm::normalize(glm::vec3(Random::randomFloat(-1, 1), Random::randomFloat(-1, 1), Random::randomFloat(-1, 1)));

	glm::vec3 position = playerPosition + randomDistance * randomDirection;
	glm::vec3 velocity = -randomDirection * randomVelocity;

	transform.TranslateTo(position);
	vel = velocity;
	velToUpdate = vel;
	angVel = randomRotationAxis * randomAngularVelocity;
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;
	//Add collider
	setCollider(scale, 0x2, 0x7); // Layer = 0b00000010 Mask = 0b00000111
	setModel(getModelName(), vertexDescriptor);
}

void AbstractAsteroidObject::Update() {
	vel = velToUpdate;
	transform.TranslateBy(vel * Time::getDeltaT());
	transform.RotateBy(angVel * Time::getDeltaT());
	if (scale != scaleToUpdate) {
		scale = scaleToUpdate;
		transform.ScaleTo(glm::vec3(scale));
		collider->setRadius(scale);
	}
}

void AbstractAsteroidObject::OnCollisionWith(GameObject* other) {
	if (other->collider != nullptr) {
		switch (other->collider->getCollisionLayer()) {
		case 0x1: {
			// Collision with the spaceship - bounce (as if the other asteroid had infinite mass & zero velocity) & receive damage
			SpaceshipObject* spaceship = (SpaceshipObject*)other;
			glm::vec3 otherVelocity = spaceship->getVelocity();
			float otherMass = 20.f;
			float mass = collider->getRadius();
			velToUpdate = ((2 * otherMass * otherVelocity) + (vel * (mass - otherMass))) / (mass + otherMass);
			float relativeVelMagnitude = length(otherVelocity - vel);
			receiveDamage(3 * max(relativeVelMagnitude - 5, 0.f));
			break;
		}
		case 0x2: {
			// Collision with another asteroid - perfectly elastic collision
			AbstractAsteroidObject* otherAsteroid = (AbstractAsteroidObject*)other;
			glm::vec3 otherVelocity = otherAsteroid->vel;
			float otherMass = otherAsteroid->collider->getRadius();
			float mass = collider->getRadius();
			velToUpdate = ((2 * otherMass * otherVelocity) + (vel * (mass - otherMass))) / (mass + otherMass);
			break;
		}
		case 0x4: {
			// Collision with a bullet
			Pew* pew = (Pew*)other;
			receiveDamage(pew->getDamage());
			break;
		}
		default:
			//Ignore other collisions (but this will never happen since they are already filtered beforehand :))
			break;
		}
	}
}

void AbstractAsteroidObject::receiveDamage(float damage) {
	// new scale^3 = scale^3 - (3/4pi)*damage, scales volume linearly wrt to damage
	scaleToUpdate = std::cbrtf(std::pow(scale, 3) - 0.238732414637843f * damage);
	// destroy myself if i'm too small
	if (scaleToUpdate < minScale)
	parentScene->removeObject(this);
}