#include "../../../headers/engine/objects/Camera.hpp"

Camera::Camera(float FOVy, float nearPlane, float farPlane, float* ar) {
    this->FOVy = FOVy;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    aspectRatio = ar;
}

void Camera::setTarget(Transform* targetTransform) {
    this->target = targetTransform;
}

void Camera::setTargetDistance(vec3 targetDistance) {
    this->distance = targetDistance;
    targetMat = target->getMatrix();
    dampedPos = glm::vec3(targetMat * glm::vec4(distance, 1));
    camPos = dampedPos;
}

void Camera::reset() {
    targetMat = target->getMatrix();
    dampedPos = glm::vec3(targetMat * glm::vec4(distance, 1));
    camPos = dampedPos;
}

glm::mat4 Camera::getCameraMatrix() {
    targetMat = target->getMatrix();
    uy = glm::normalize(glm::vec3(targetMat * glm::vec4(0, 1, 0, 0)));
  
    camTarget = target->getPos() + 2.0f*uy;
    camPos = glm::vec3(targetMat * glm::vec4(distance, 1));

    dampedPos = DAMP(dampedPos, camPos, Time::getDeltaT());

    prj = glm::perspective(FOVy, *aspectRatio, nearPlane, farPlane);
    prj[1][1] *= -1;

    view = glm::lookAt(dampedPos, camTarget, uy);

    return prj * view;
}

glm::vec3 Camera::getCameraPosition() {
    return dampedPos;
}