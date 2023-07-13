#ifndef TRANSFORM_INCLUDE
#define TRANSFORM_INCLUDE

#include "../base/includes.hpp"

using namespace glm;

class Transform {
    protected:
        mat4 translationMatrix = mat4(1);
        mat4 rotationMatrix = mat4(1);
        mat4 scaleMatrix = mat4(1);

    public:
        Transform(mat4 t, mat4 r, mat4 s);
        Transform() {
            translationMatrix = mat4(1);
            rotationMatrix = mat4(1);
            scaleMatrix = mat4(1);
        };
        void TranslateTo(vec3 to);
        void TranslateBy(vec3 by);
        void TranslateLocalBy(vec3 by);
        void Rotate(float by, vec3 around);
        void RotateBy(vec3 by);
        void Scale(vec3 by);

        mat4 getMatrix();

        static Transform identity();
};

#endif