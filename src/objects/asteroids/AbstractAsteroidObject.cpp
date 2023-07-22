#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/base/Time.hpp"
#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/AbstractAsteroidObject.hpp"
#include "../../../headers/objects/Pew.hpp"
#include "../../../headers/engine/base/Random.hpp"
#include <headers/objects/SpaceshipObject.hpp>

AbstractAsteroidObject::AbstractAsteroidObject(Transform* _playerTransform) {
	transform = Transform::identity();
	scaleToUpdate = Random::randomFloat(1.25f, 6);
	minScale = std::min(scaleToUpdate, 1.5f);
	playerTransform = _playerTransform;
}

void AbstractAsteroidObject::initializeRandom() {
	//Position the asteroid in a random point around the player at a random distance and with random velocities
	glm::vec3 randomDirection = glm::normalize(glm::vec3(Random::randomFloat(-1, 1), Random::randomFloat(-1, 1), Random::randomFloat(-1, 1)));
	float randomDistance = Random::randomFloat(80, 160);
	float randomVelocity = Random::randomFloat(1.5f, 8);
	float randomAngularVelocity = Random::randomFloat(10, 90);
	glm::vec3 randomRotationAxis = glm::normalize(glm::vec3(Random::randomFloat(-1, 1), Random::randomFloat(-1, 1), Random::randomFloat(-1, 1)));

	glm::vec3 position = playerTransform->getPos() + randomDistance * randomDirection;
	glm::vec3 velocity = -randomDirection * randomVelocity;

	transform.TranslateTo(position);
	vel = velocity;
	velToUpdate = vel;
	angVel = randomRotationAxis * randomAngularVelocity;

	scale = 0.0f; // pop "animation"
	transform.ScaleTo(glm::vec3(scale));
}

void AbstractAsteroidObject::Instantiate() {
	// Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(AbstractAsteroidVertex), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AbstractAsteroidVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(AbstractAsteroidVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AbstractAsteroidVertex, norm), sizeof(glm::vec3), NORMAL);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(AbstractAsteroidVertex, tan), sizeof(glm::vec4), TANGENT);
	initializeRandom();
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;
	//Add collider
	addCollider(glm::vec3(0), scale, 0x2, 0x7); // Layer = 0b00000010 Mask = 0b00000111
	setModel(getModelName(), vertexDescriptor);
}

void AbstractAsteroidObject::Update() {
	float delT = Time::getDeltaT();
	vel = velToUpdate;
	transform.TranslateBy(vel * delT);
	transform.RotateBy(angVel * delT);
	scale = DAMP(scale, scaleToUpdate, delT);
	transform.ScaleTo(glm::vec3(scale));
	colliders[0]->setRadius(scale);
	if (glm::distance(transform.getPos(), playerTransform->getPos()) > 300) {
		initializeRandom();
	}
}

void AbstractAsteroidObject::OnCollisionWith(Collider* other) {
	if (other != nullptr) {
		GameObject* parent = other->getParent();
		switch (other->getCollisionLayer()) {
		case 0x1: {
			// Collision with the spaceship - bounce (as if the other asteroid had infinite mass & zero velocity) & receive damage
			SpaceshipObject* spaceship = (SpaceshipObject*)parent;
			glm::vec3 otherVelocity = spaceship->getVelocity();
			float otherMass = 20.f;
			float mass = colliders[0]->getRadius();
			velToUpdate = ((2 * otherMass * otherVelocity) + (vel * (mass - otherMass))) / (mass + otherMass);
			float relativeVelMagnitude = length(otherVelocity - vel);
			receiveDamage(3 * max(relativeVelMagnitude - 5, 0.f));
			break;
		}
		case 0x2: {
			// Collision with another asteroid - perfectly elastic collision
			AbstractAsteroidObject* otherAsteroid = (AbstractAsteroidObject*)parent;
			glm::vec3 otherVelocity = otherAsteroid->vel;
			float otherMass = otherAsteroid->colliders[0]->getRadius();
			float mass = colliders[0]->getRadius();
			velToUpdate = ((2 * otherMass * otherVelocity) + (vel * (mass - otherMass))) / (mass + otherMass);
			break;
		}
		case 0x4: {
			// Collision with a bullet
			Pew* pew = (Pew*)parent;
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
	scaleToUpdate = std::cbrtf(std::pow(scaleToUpdate, 3) - 0.238732414637843f * damage);
	// destroy myself if i'm too small
	if (scaleToUpdate < minScale)
		parentScene->removeObject(this);
}