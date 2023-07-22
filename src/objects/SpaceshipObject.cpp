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

	initialScale = 0.3f;
	transform.ScaleTo(glm::vec3(initialScale));

	vel = glm::vec3(0.0f);
	angVel = glm::vec3(0.0f);

	addCollider(glm::vec3(0, -0.47, -14.26), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -12.32), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -10.48), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -8.72), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -6.72), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -4.82), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, -0.47, -2.84), 1.271f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0), 2.5172f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(-5.83, 0, 2.03), 3.1f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(5.83, 0, 2.03), 3.1f * initialScale, 0x01, 0x02);
	addCollider(glm::vec3(0, 0, 5.18), 3.1f * initialScale, 0x01, 0x02);
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = true;

	additionalUniforms.emissionColor = glm::vec4(1.f, 0.723f, 0.022f, 0.f);

	// initialize all the effect timers to 0
	effectTimers = std::vector<float>(sizeof(Effect), 0.0f);
}

void SpaceshipObject::Update() {
	float delT = Time::getFixedDeltaT(); // always with scale 1

	// effect timers
	for (int i = 0; i < sizeof(Effect); i++) {
		if (hasEffect((Effect)i)) { // effect i enabled
			effectTimers[i] += delT; // increment effect i timer
			if (effectTimers[i] > effectDuration) {
				unsetEffect((Effect)i); // unset effect
				effectTimers[i] = 0; // reset the timer to zero
			}
		}
	}

	// check if has to slow down time
	float timeScale;
	if (!hasEffect(EFFECT_TIME)) {
		timeScale = DAMP(Time::getScale(), 1.0f, delT);
	}
	else {
		timeScale = DAMP(Time::getScale(), effectTimeScale, delT);
	}
	Time::setScale(timeScale);

	// check if scaleDown effect
	float scale = transform.getScale().x, newScale; // should be all equal on x,y,z
	if (!hasEffect(EFFECT_SIZEDOWN)) {
		newScale = DAMP(scale, initialScale, delT);
	}
	else {
		newScale = DAMP(scale, initialScale / effectScaleDownFactor, delT);
	}
	if (newScale != scale) {
		transform.ScaleTo(glm::vec3(newScale));
		scaleColliders(newScale);
		scale = newScale;
	}

	glm::vec3 mov, rot;
	Inputs::getSixAxis(mov, rot, fire);

	// blocking some movements
	mov.x = 0;
	mov.y = 0;
	mov.z = std::max(mov.z, 0.0f);

	if (disabledKeysTimer > 0) {
		disabledKeysTimer -= delT;
		mov = glm::vec3(0);
		rot = glm::vec3(0);
		additionalUniforms.flashingColor = baseFlashingColor * sin(disabledKeysTimer * 5);
	}
	else {
		additionalUniforms.flashingColor = glm::vec4(0);
	}

	emissionMultiplier = std::min(1.0f, mov.z);
	additionalUniforms.emissionColor.a = DAMP(additionalUniforms.emissionColor.a, emissionMultiplier, delT);

	firingAllowed = firingAllowed || !fire;
	fire = fire && firingAllowed;

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
	}
	else if (fire) {
		reloading = true;
		glm::vec3 finalShotSpeed = glm::vec3(0, 0, -shotSpeed) + vel; // negative z for the same reason mov.z is multiplied by -1
		glm::vec3 shotColor = this->shotColor;

		float shotDamage = this->shotDamage;
		float shotThickness = this->shotThickness * scale; // scale with spaceShip scale
		glm::vec3 shotOffset = this->shotOffset * scale;

		// check if damage effect
		if (hasEffect(EFFECT_DAMAGE)) {
			shotDamage *= effectDamageMultiplier;
			shotThickness *= effectDamageMultiplier;
			shotColor = glm::vec3(0.1f, 0.5f, 1);
		}


		if (hasEffect(EFFECT_FIRERATE)) { // shot one more shot
			shotColor = glm::vec3(0.5f, 0, 0.5f);
			Pew* pew = new Pew(transform, shotOffset, finalShotSpeed, shotRange, shotDamage, shotThickness, shotColor);
			pew->Instantiate();
			parentScene->addObject(pew);
			shotOffset.x *= -1; // spawn pews in both directions
		}
		Pew* pew = new Pew(transform, shotOffset, finalShotSpeed, shotRange, shotDamage, shotThickness, shotColor);
		pew->Instantiate();
		parentScene->addObject(pew);
		this->shotOffset.x *= -1; // pew left / right
	}
}

void SpaceshipObject::OnCollisionWith(Collider* other) {
	//Collision is assumed to be only with asteroids
	if (!hadRecentCollision()) {
		lives -= 1;
		// shut down light
		additionalUniforms.emissionColor.a = 0.0f;
		if (lives <= 0) {
			parentScene->requestSceneSwitch(AR_SCENE_GAME_OVER);
			disabledKeysTimer = 0;
			firingAllowed = false;
			vel = glm::vec3(0.0f);
			angVel = glm::vec3(0.0f);
			// initialize all the effect timers to 0
			effectTimers = std::vector<float>(sizeof(Effect), 0.0f);
			effects = 0;
			transform.ScaleTo(glm::vec3(initialScale));
			scaleColliders(initialScale);
		}
		else {
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

bool SpaceshipObject::hasEffect(Effect e) {
	return effects & (1 << e);
}

void SpaceshipObject::setEffect(Effect e) {
	effects |= 1 << e;
	effectTimers[e] = 0.0f;
}

void SpaceshipObject::unsetEffect(Effect e) {
	effects &= ~(1 << e); // unset the effect flag
}

void SpaceshipObject::scaleColliders(float scale) {
	static float colliderScales[11] = { 1.271f, 1.271f, 1.271f, 1.271f, 1.271f, 1.271f, 1.271f, 2.5172f, 3.1f, 3.1f, 3.1f };
	for (int i = 0; i < 11; i++) {
		colliders[i]->setRadius(colliderScales[i] * scale);
	}
}