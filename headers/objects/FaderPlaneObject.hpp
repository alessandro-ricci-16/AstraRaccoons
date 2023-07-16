#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_FADERPLANEOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_FADERPLANEOBJECT_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/Camera.hpp"
#include "../engine/objects/MeshObject.hpp"

struct FaderPlaneVertex {
    glm::vec3 pos;
};

class FaderPlaneObject : public MeshObject<FaderPlaneVertex> {
    public:
        void Instantiate();
        void Start();
        void Update();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_FADERPLANEOBJECT_HPP_