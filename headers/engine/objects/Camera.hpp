#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_

#include "../base/includes.hpp"
#include "Transform.hpp"

class Camera {
    private:
        float FOVy;
        float nearPlane;
        float farPlane;
        Transform* target;
        float distance;
        float* aspectRatio;

    public:
        Camera() = default;
        Camera(float FOVy, float nearPlane, float farPlane, float* aspectRatio);

        void setTarget(Transform* targetTransform);
        void setTargetDistance(float targetDistance);

        glm::mat4 getCameraMatrix(); //Prj * View
        glm::vec3 getCameraPosition();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_