#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/engine/base/libraries.hpp"
#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/SpaceshipObject.hpp"
#include "../../headers/engine/base/Inputs.hpp"
#include "../../headers/objects/Pew.hpp"

void SpaceshipObject::Instantiate() {
	// Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(SpaceshipVertex), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SpaceshipVertex, pos), sizeof(glm::vec3), POSITION);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32_SFLOAT, offsetof(SpaceshipVertex, uv), sizeof(glm::vec2), UV);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SpaceshipVertex, norm), sizeof(glm::vec3), NORMAL);
	setModel("models/spaceship.obj", vertexDescriptor);
	model.setShader("shaders/ShipShader_Vert.spv", "shaders/ShipShader_Frag.spv");
	model.addTexture("textures/spaceship/Albedo.png");
	model.addTexture("textures/spaceship/MetallicRoughnessEmission.png");
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
	transform.Scale(glm::vec3(0.3f));
	vel = glm::vec3(0.0f);
	angVel = glm::vec3(0.0f);
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;
}

void SpaceshipObject::Start() {}

void SpaceshipObject::Update() {
	float delT = Time::getDeltaT();
	glm::vec3 mov, rot;
	Inputs::getSixAxis(mov, rot, fire);
	rot *= glm::vec3(-1, -1, 1);
	mov *= glm::vec3(1, 1, -1);
	// increments velocity by movement * acceleration, decrements where there is no movement linearly to velocity
	vel += (mov * acc - dec * vel) * delT;
	angVel += (rot * angAcc - angDec * angVel) * delT;
	transform.TranslateLocalBy(vel * delT);
	transform.RotateBy(angVel * delT);
	if (reloading) {
		timer += delT;
		if (timer >= fireRate) {
			timer = 0;
			reloading = false;
		}
	}
	else if (fire) {
		reloading = true;
		Pew* pew = new Pew(transform, shotSpeed, shotRange, shotDamage, glm::vec3(1, 0, 0));
		pew->Instantiate();
		parentScene->addObject(pew);
	}
}

void SpaceshipObject::OnCollisionWith(GameObject* other) {

}