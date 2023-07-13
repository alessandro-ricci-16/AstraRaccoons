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
    private:
        glm::vec3 vel;
        glm::vec3 angVel;
        const float maxVel = 10.0f;
        const float maxAngVel = 180.0f;
        const float acc = 5.0f;
        const float angAcc = 90.0f;

    public:
        void Instantiate();
        void Start();
        void Update();
        void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_