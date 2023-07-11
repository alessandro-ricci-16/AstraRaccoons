#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/objects/MeshObject.hpp>

struct SpaceshipVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class SpaceshipObject : public MeshObject<SpaceshipVertex>, public ICollidable {
    public:
        void Instantiate();
        void Start();
        void Update();
        void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_