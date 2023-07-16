#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_ORBITINGCAMERA_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_ORBITINGCAMERA_HPP_

#include "Camera.hpp"

class OrbitingCamera: public Camera {
    public:
        using Camera::Camera;

        glm::vec3 getCameraPosition() override;
        glm::mat4 getCameraMatrix() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_OBJECTS_ORBITINGCAMERA_HPP_