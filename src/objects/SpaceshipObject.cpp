#define MESHOBJECT_IMPLEMENTATION
#include "../../headers/engine/base/libraries.hpp"
#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/SpaceshipObject.hpp"
#include "../../headers/engine/base/Inputs.hpp"
#include "../../headers/objects/Pew.hpp"
#include <headers/Game.hpp>
#include <headers/scenes/AvailableScenes.hpp>

glm::vec3 SpaceshipObject::getVelocity() {
	return vel;
}

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
	model.addUniformData(&additionalUniforms, sizeof(SpaceshipUniforms), VK_SHADER_STAGE_FRAGMENT_BIT);
	const std::vector<std::string> textures = { "textures/sky/right.png", "textures/sky/left.png",
							  "textures/sky/top.png",   "textures/sky/bottom.png",
							  "textures/sky/back.png", "textures/sky/front.png" };
	model.addCubicTexture(textures);
	float scale = 0.3f;
	transform.Scale(glm::vec3(scale));
	vel = glm::vec3(0.0f);
	angVel = glm::vec3(0.0f);
	addCollider(glm::vec3(0, -0.47, -14.26), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -12.32), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -10.48), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -8.72), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -6.72), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -4.82), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -2.84), 2.05f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0), 4.06f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(-5.83, 0, 2.03), 5.f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(5.83, 0, 2.03), 5.f * 0.62f * scale, 0x01, 0x02);
	addCollider(glm::vec3(0, 0, 5.18), 5.f * 0.62f * scale, 0x01, 0x02);
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;
}

void SpaceshipObject::Update() {
	float delT = Time::getFixedDeltaT(); // always with scale 1
	disabledKeysTimer -= delT;

	glm::vec3 mov, rot;
	Inputs::getSixAxis(mov, rot, fire);
	if (disabledKeysTimer > 0) {
		mov = glm::vec3(0);
		rot = glm::vec3(0);
		additionalUniforms.flashingColor = baseFlashingColor * sin(disabledKeysTimer * 5);
	} else {
		additionalUniforms.flashingColor = glm::vec4(0);
	}

	rot *= glm::vec3(-1, -1, 1);
	mov.z *= -1;

	// increments velocity by movement * acceleration, decrements where there is no movement linearly to velocity
	vel += (mov * acc - dec * vel) * delT;
	angVel += (rot * angAcc - angDec * angVel) * delT;

	transform.TranslateLocalBy(vel * delT);
	transform.RotateBy(angVel * delT);

	// shooting
	if (reloading) {
		// wait fireRate time before beeing ready to shot another pew
		timer += delT;
		if (timer >= fireRate) {
			timer = 0;
			reloading = false;
		}
	} else if (fire) {
		reloading = true;
		glm::vec3 finalShotSpeed = glm::vec3(0, 0, -shotSpeed) + vel; // negative z for the same reason mov.z is multiplied by -1
		Pew* pew = new Pew(transform, shotOffset, finalShotSpeed, shotRange, shotDamage, shotThickness, shotColor);
		pew->Instantiate();
		parentScene->addObject(pew);
		shotOffset.x *= -1; // pew left / right
	}
}

void SpaceshipObject::OnCollisionWith(Collider* other) {
	//Collision is assumed to be only with asteroids
	if (!hadRecentCollision()) {
		lives -= 1;
		if (lives <= 0) {
			parentScene->requestSceneSwitch(AR_SCENE_GAME_OVER);
			disabledKeysTimer = 0;
		} else {
			disabledKeysTimer = disabledKeysDefaultTimer;
		}
	}
}

void SpaceshipObject::resetLives() {
	lives = maxLives;
}

bool SpaceshipObject::hadRecentCollision() {
	return disabledKeysTimer > 0;
}
