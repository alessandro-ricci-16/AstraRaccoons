#define MESHOBJECT_IMPLEMENTATION
#include <ctime>

#include "../../headers/engine/base/Time.hpp"
#include "../../headers/engine/scenes/Scene.hpp"
#include "../../headers/objects/Pew.hpp"

Pew::Pew(Transform parent, glm::vec3 offset, glm::vec3 _speed, float _range, float _damage, float _thickness, glm::vec3 _color) {
	transform = parent;
	transform.ScaleTo(glm::vec3(1, 1, 1));
	transform.TranslateLocalBy(offset);
	initialPos = transform.getPos();
	speed = _speed;
	range = _range;
	damage = _damage;
	thickness = _thickness;
	color = { _color };
}

void Pew::Instantiate() {
	// Load the model component
	ObjectVertexDescriptor* vertexDescriptor = new ObjectVertexDescriptor();
	vertexDescriptor->addBinding(sizeof(PewVertex), true);
	vertexDescriptor->addLocation(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(PewVertex, pos), sizeof(glm::vec3), POSITION);
	float length = 4 * thickness;
	std::vector<PewVertex> verts = { {glm::vec3(-thickness / 2, -thickness / 2, 0)},
										   {glm::vec3(thickness / 2, -thickness / 2, 0)},
											{glm::vec3(thickness / 2, thickness / 2, 0)},
										   {glm::vec3(-thickness / 2, thickness / 2, 0)},
											{glm::vec3(-thickness / 2, -thickness / 2, -length)},
										   {glm::vec3(thickness / 2, -thickness / 2, -length)},
											{glm::vec3(thickness / 2, thickness / 2, -length)},
										   {glm::vec3(-thickness / 2, thickness / 2, -length)} };
	std::vector<uint32_t> indices = { 3, 2, 1,
									  1, 0, 3, // front face
									  4, 5, 6,
									  6, 7, 4, // back face
									  0, 4, 7,
									  7, 3, 0, // left face
									  3, 1, 6,
									  6, 2, 3, // up face
									  1, 2, 6,
									  6, 5, 1, // right face
									  0, 1, 5,
									  5, 4, 0, // bottom face
	};
	setModel(verts, indices, vertexDescriptor);
	model.setShader("shaders/Pew_Vert.spv", "shaders/Pew_Frag.spv");
	model.addUniformData(&color, sizeof(PewUniform), VK_SHADER_STAGE_FRAGMENT_BIT);
	model.isTransparent = false;
	model.backfaceCullingOn = true;
	// Enable GUBOs -- REQUIRED if the shader uses them!
	acceptsGUBOs = false;
}

void Pew::Start() {}

void Pew::Update() {
	transform.TranslateLocalBy(speed * Time::getDeltaT());
	if (glm::length(transform.getPos() - initialPos) >= range) {
		// kill the pew when it reaches the full range
		parentScene->removeObject(this);
	}
}