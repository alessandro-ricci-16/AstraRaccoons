#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../base/Time.hpp"

class Camera {
    protected:
        Transform* target = nullptr;
        float FOVy;
        float nearPlane;
        float farPlane;
        float* aspectRatio = nullptr; 
        glm::vec3 camPos;
        glm::vec3 camTarget;
        glm::vec3 dampedPos;
        glm::vec3 distance;
        float targetYOffset = 0;
        glm::vec3 uy;
        glm::mat4 prj;
        glm::mat4 targetMat;
        glm::mat4 view;

    public:
        Camera() = default;
        Camera(float FOVy, float nearPlane, float farPlane, float* aspectRatio);

        void setTarget(Transform* targetTransform);
        void setTargetDistance(vec3 targetDistance);
        void setTargetYOffset(float targetYOffset);
        void reset();

        virtual glm::vec3 getCameraPosition();
        virtual glm::mat4 getCameraMatrix(); //Prj * View
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_