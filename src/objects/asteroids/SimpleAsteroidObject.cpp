#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include <headers/engine/base/Time.hpp>
#include <headers/engine/scenes/Scene.hpp>
#include <headers/objects/asteroids/SimpleAsteroidObject.hpp>
#include <headers/engine/base/Random.hpp>

SimpleAsteroidObject::SimpleAsteroidObject(Transform* playerTransform) {
    transform = Transform::identity();
    this->playerTransform = playerTransform;
    acceptsGUBOs = true;
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
    const std::vector<std::string> textures = {"textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
	transform.Scale(glm::vec3(3));
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
	angVel = randomRotationAxis * randomAngularVelocity;
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;
}

void SimpleAsteroidObject::Start() {

}

void SimpleAsteroidObject::Update() {
    transform.TranslateBy(vel * Time::getDeltaT());
	transform.RotateBy(angVel * Time::getDeltaT());
}

void SimpleAsteroidObject::OnCollisionWith(GameObject* other) {}