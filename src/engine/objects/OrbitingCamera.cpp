#include <headers/engine/objects/OrbitingCamera.hpp>

glm::mat4 OrbitingCamera::getCameraMatrix() {
    prj = glm::perspective(FOVy, *aspectRatio, nearPlane, farPlane);
    prj[1][1] *= -1;
    camTarget = target->getPos();
    camPos = glm::vec3(0);
    view = glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));
    return prj * view;
}

glm::vec3 OrbitingCamera::getCameraPosition() {
    return camPos;
}