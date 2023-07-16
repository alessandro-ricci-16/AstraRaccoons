#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../base/Time.hpp"

#define LAMBDA 3.f
#define LERP(start, end, t) start * t + end * (1 - t)
#define LERPTIME(dt) (float)pow(2.718281828459045f, -LAMBDA * dt)
#define DAMP(start, end, dt) LERP(start, end, LERPTIME(dt));

class Camera {
    protected:
        Transform* target;
        float FOVy;
        float nearPlane;
        float farPlane;
        float* aspectRatio; 
        glm::vec3 camPos;
        glm::vec3 camTarget;
        glm::vec3 dampedPos;
        glm::vec3 distance;
        glm::vec3 uy;
        glm::mat4 prj;
        glm::mat4 targetMat;
        glm::mat4 view;

    public:
        Camera() = default;
        Camera(float FOVy, float nearPlane, float farPlane, float* aspectRatio);

        void setTarget(Transform* targetTransform);
        void setTargetDistance(vec3 targetDistance);

        virtual glm::vec3 getCameraPosition();
        virtual glm::mat4 getCameraMatrix(); //Prj * View
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_CAMERA_HPP_