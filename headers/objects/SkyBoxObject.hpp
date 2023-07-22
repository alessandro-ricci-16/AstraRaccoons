#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SKYBOXOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SKYBOXOBJECT_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"
#include "../engine/objects/Camera.hpp"

struct SkyBoxObjectVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
};

class SkyBoxObject : public MeshObject<SkyBoxObjectVertex> {
private:
	Camera* camera;
public:
	SkyBoxObject(Camera* c);
	void Instantiate();
	void Update();
};

#endif  // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SKYBOXOBJECT_HPP_