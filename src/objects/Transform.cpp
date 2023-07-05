#include "../../headers/objects/Transform.hpp"

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

void Transform::Rotate(float by, vec3 around) {
    rotationMatrix = rotate(rotationMatrix, radians(by), around);
}

void Transform::Scale(vec3 by) {
    scaleMatrix = scale(scaleMatrix, by);
}

mat4 Transform::getMatrix() {
    return translationMatrix * rotationMatrix * scaleMatrix;
}

Transform Transform::identity() {
    return Transform(mat4(1), mat4(1), mat4(1));
}