#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/Camera.hpp"
#include "../engine/objects/MeshObject.hpp"
#include <headers/engine/objects/Collider.hpp>

struct PewVertex {
    glm::vec3 pos;
};

struct PewUniform {
    glm::vec3 color;
};

class Pew : public MeshObject<PewVertex>, public ICollidable {
    private:
        PewUniform color;
        glm::vec3 speed;
        glm::vec3 initialPos;
        float range;
        float damage;
        float thickness;
    public:
        Pew(Transform parent, glm::vec3 offset, glm::vec3 speed, float range, float damage, float thickness, glm::vec3 color);
        float getDamage();
        void Instantiate();
        void Update();
        void OnCollisionWith(Collider* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_PEW_HPP_