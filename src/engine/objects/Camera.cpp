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
    distance = targetDistance;
    targetMat = target->getMatrix();

    dampedPos = glm::vec3(targetMat * glm::vec4(distance / target->getScale(), 1));
    camPos = dampedPos;
}

void Camera::setTargetYOffset(float _targetYOffset) {
    targetYOffset = _targetYOffset;
}

void Camera::reset() {
    targetMat = target->getMatrix();
    dampedPos = glm::vec3(targetMat * glm::vec4(distance, 1));
    camPos = dampedPos;
}

glm::mat4 Camera::getCameraMatrix() {
    targetMat = target->getMatrix();
    glm::vec3 targetScale = target->getScale();
    uy = target->uy();
  
    camTarget = target->getPos() + targetYOffset * uy * targetScale.y;
    camPos = glm::vec3(targetMat * glm::vec4(distance / targetScale, 1));

    dampedPos = DAMP(dampedPos, camPos, Time::getFixedDeltaT());

    prj = glm::perspective(FOVy, *aspectRatio, nearPlane, farPlane);
    prj[1][1] *= -1;

    view = glm::lookAt(dampedPos, camTarget, uy);

    return prj * view;
}

glm::vec3 Camera::getCameraPosition() {
    return dampedPos;
}