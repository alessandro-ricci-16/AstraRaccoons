#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/Camera.hpp"
#include "../engine/objects/MeshObject.hpp"

struct PewVertex {
    glm::vec3 pos;
};

struct PewUniform {
    glm::vec3 color;
};

class Pew : public MeshObject<PewVertex> {
    private:
        PewUniform color;
        glm::vec3 speed;
        glm::vec3 initialPos;
        float range;
        float damage;
    public:
        Pew(Transform parent, float speed, float range, float damage, glm::vec3 color);
        void Instantiate();
        void Start();
        void Update();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_