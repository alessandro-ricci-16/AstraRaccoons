#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include "../../../headers/engine/base/Time.hpp"
#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/SimpleAsteroidObject.hpp"
#include "../../../headers/objects/Pew.hpp"
#include "../../../headers/engine/base/Random.hpp"

SimpleAsteroidObject::SimpleAsteroidObject(Transform* playerTransform, float _initialScale) {
	transform = Transform::identity();
	scale = _initialScale;
	scaleToUpdate = scale;
	transform.ScaleTo(glm::vec3(scale));
	this->playerTransform = playerTransform;
}

void SimpleAsteroidObject::Instantiate() {
	// Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(SimpleAsteroidVertex), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SimpleAsteroidVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(SimpleAsteroidVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SimpleAsteroidVertex, norm), sizeof(glm::vec3), NORMAL);
	setModel("models/Sphere.gltf", vertexDescriptor);
	model.setShader("shaders/asteroids/SimpleAsteroidShader_Vert.spv", "shaders/asteroids/SimpleAsteroidShader_Frag.spv");
	model.addTexture("textures/asteroids/copper/Albedo.png");
	model.addTexture("textures/asteroids/copper/RoughnessMetalness.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
	//Position the asteroid in a random point around the player at a random distance and with random velocities
	glm::vec3 playerPosition = transform.getPos();
	glm::vec3 randomDirection = glm::normalize(glm::vec3(Random::randomFloat(0, 1), Random::randomFloat(0, 1), Random::randomFloat(0, 1)));
	float randomDistance = Random::randomFloat(80, 100);
	float randomVelocity = Random::randomFloat(0.8, 5);
	float randomAngularVelocity = Random::randomFloat(2, 10);
	glm::vec3 randomRotationAxis = glm::normalize(glm::vec3(Random::randomFloat(0, 1), Random::randomFloat(0, 1), Random::randomFloat(0, 1)));

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
}

void SimpleAsteroidObject::Start() {

}

void SimpleAsteroidObject::Update() {
	vel = velToUpdate;
	transform.TranslateBy(vel * Time::getDeltaT());
	transform.RotateBy(angVel * Time::getDeltaT());
	if (scale != scaleToUpdate) {
		scale = scaleToUpdate;
		transform.ScaleTo(glm::vec3(scale));
		collider->setRadius(scale);
	}
}

void SimpleAsteroidObject::OnCollisionWith(GameObject* other) {
	if (other->collider != nullptr) {
		switch (other->collider->getCollisionLayer()) {
		case 0x1: {
			// Collision with the spaceship
			break;
		}
		case 0x2: {
			// Collision with another asteroid - perfectly elastic collision
			SimpleAsteroidObject* otherAsteroid = (SimpleAsteroidObject*)other;
			glm::vec3 otherVelocity = otherAsteroid->vel;
			float otherMass = otherAsteroid->collider->getRadius();
			float mass = collider->getRadius();
			velToUpdate = ((2 * otherMass * otherVelocity) + (vel * (mass - otherMass))) / (mass + otherMass);
			break;
		}
		case 0x4: {
			// Collision with a bullet
			Pew* pew = (Pew*)other;
			// new scale^3 = scale^3 - (3/4pi)*damage, scales volume linearly wrt to damage
			scaleToUpdate = std::cbrtf(std::pow(scale, 3) - 0.238732414637843f * pew->getDamage());
			// destroy myself if i'm too small
			if (scaleToUpdate < minScale)
				parentScene->removeObject(this);
			break;
		}
		default:
			//Ignore other collisions (but this will never happen since they are already filtered beforehand :))
			break;
		}
	}
}