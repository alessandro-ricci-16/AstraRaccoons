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
        // maxVel = a/d
        // acc = a - d*v
        const float acc = 15.0f;
        const float dec = 1.0f;
        const float angAcc = 250.0f;
        const float angDec = 2.0f;

        bool fire = false;
        bool reloading = false;
        float timer = 0.0f;

        const float fireRate = 0.2f;
        const float shotSpeed = 75.0f;
        const float shotRange = 100.0f;
        const float shotDamage = 10.0f;
        const float shotThickness = 0.2f;

        const glm::vec3 shotColor = glm::vec3(1, 0, 0);
        glm::vec3 shotOffset = glm::vec3(1.3f, -0.1f, 0.7f);

    public:
        void Instantiate();
        void Start();
        void Update();
        void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_