#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SIMPLEASTEROIDOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SIMPLEASTEROIDOBJECT_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/objects/MeshObject.hpp>

struct SimpleAsteroidVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class SimpleAsteroidObject : public MeshObject<SimpleAsteroidVertex>, public ICollidable {
    private:
        glm::vec3 velToUpdate;
        glm::vec3 vel;
        glm::vec3 angVel;
        Transform* playerTransform;

    public:
        SimpleAsteroidObject(Transform* playerTransform);

        void Instantiate();
        void Start();
        void Update();
        void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SIMPLEASTEROIDOBJECT_HPP_