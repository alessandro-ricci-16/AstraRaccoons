#include "../../../headers/engine/objects/OrbitingCamera.hpp"
#include "../../../headers/engine/base/Time.hpp"

glm::mat4 OrbitingCamera::getCameraMatrix() {
	prj = glm::perspective(FOVy, *aspectRatio, nearPlane, farPlane);
	prj[1][1] *= -1;
	camTarget = target->getPos();
	view = glm::lookAt(getCameraPosition(), camTarget, glm::vec3(0, 1, 0));
	return prj * view;
}

glm::vec3 OrbitingCamera::getCameraPosition() {
	float absTime = Time::getAbsoluteTime();
	return target->getPos() + glm::vec3(glm::cos(0.35f * absTime), 0, glm::sin(0.35f * absTime)) * distance + glm::vec3(0, distance.y, 0);
}