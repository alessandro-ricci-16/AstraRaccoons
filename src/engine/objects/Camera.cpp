#include "../../../headers/engine/objects/Camera.hpp"

Camera::Camera(float FOVy, float nearPlane, float farPlane) {
    this->FOVy = FOVy;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
}

void Camera::setTarget(Transform* targetTransform) {
    this->target = targetTransform;
}

void Camera::setTargetDistance(float targetDistance) {
    this->distance = targetDistance;
}

glm::mat4 Camera::getCameraMatrix(float aspectRatio) {
    //Get the projection
    /*glm::mat4 Prj = glm::perspective(FOVy, aspectRatio, nearPlane, farPlane);
    Prj[1][1] *= -1;
    //Get the View
    glm::vec3 camTarget = target->getMatrix() * glm::vec4(0, 0, 0, 1);
    glm::vec3 camPos = target->getMatrix() * glm::vec4(0, 0, distance, 1);
    glm::mat4 View = glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));*/

    glm::mat4 Prj = glm::perspective(FOVy, aspectRatio, nearPlane, farPlane);
    Prj[1][1] *= -1;
    glm::vec3 camTarget = glm::vec3(0, 0, 0);
    glm::vec3 camPos = camTarget + glm::vec3(0, 0, -2);
    glm::mat4 View = glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));
    return Prj * View;
}

glm::vec3 Camera::getCameraPosition(float aspectRatio) {
    return getCameraMatrix(aspectRatio) * glm::vec4(0, 0, 0, 1);
}