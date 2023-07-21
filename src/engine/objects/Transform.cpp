#include "../../../headers/engine/objects/Transform.hpp"

Transform::Transform(mat4 t, mat4 r, mat4 s) {
	translationMatrix = t;
	rotationMatrix = r;
	scaleMatrix = s;
}

void Transform::TranslateTo(vec3 to) {
	translationMatrix = translate(mat4(1), to);
}

void Transform::TranslateBy(vec3 by) {
	translationMatrix = translate(translationMatrix, by);
}

void Transform::TranslateLocalBy(vec3 by) {
	by = by.x * ux() + by.y * uy() + by.z * uz();
	TranslateBy(by);
}

void Transform::RotateTo(vec3 rot) {
    quat qe = angleAxis(radians(rot.z), vec3(0, 0, 1)) *
		angleAxis(radians(rot.y), vec3(0, 1, 0)) *
		angleAxis(radians(rot.x), vec3(1, 0, 0));
	rotationMatrix = mat4(qe);
}

void Transform::Rotate(float by, vec3 around) {
	rotationMatrix = rotate(rotationMatrix, radians(by), around);
}

void Transform::RotateBy(vec3 by) {
	quat qe = angleAxis(radians(by.z), vec3(0, 0, 1)) *
		angleAxis(radians(by.y), vec3(0, 1, 0)) *
		angleAxis(radians(by.x), vec3(1, 0, 0));
	rotationMatrix *= mat4(qe);
}

void Transform::Scale(vec3 by) {
	scaleMatrix = scale(scaleMatrix, by);
}

void Transform::ScaleTo(vec3 to) {
	scaleMatrix = scale(mat4(1), to);
}

mat4 Transform::getMatrix() {
	return translationMatrix * rotationMatrix * scaleMatrix;
}

vec3 Transform::getPos() {
	return vec3(translationMatrix[3]);
}

vec3 Transform::ux() {
	return vec3(rotationMatrix * vec4(1, 0, 0, 1));
}

vec3 Transform::uy() {
	return vec3(rotationMatrix * vec4(0, 1, 0, 1));
}

vec3 Transform::uz() {
	return vec3(rotationMatrix * vec4(0, 0, 1, 1));
}

vec3 Transform::getWorldPositionOfLocal(glm::vec3 localPos) {
	return getMatrix() * vec4(localPos, 1);
}

Transform Transform::identity() {
	return Transform(mat4(1), mat4(1), mat4(1));
}