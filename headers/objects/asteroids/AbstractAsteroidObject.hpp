#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/objects/MeshObject.hpp>

struct AbstractAsteroidVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class AbstractAsteroidObject : public MeshObject<AbstractAsteroidVertex>, public ICollidable {
private:
    glm::vec3 velToUpdate;
    glm::vec3 vel;
    glm::vec3 angVel;
    Transform* playerTransform;
    float scale, scaleToUpdate;
    float minScale = 1.5f;

public:
    AbstractAsteroidObject(Transform* playerTransform, float initialScale);

    void Instantiate();
    void Start();
    void Update();
    void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_