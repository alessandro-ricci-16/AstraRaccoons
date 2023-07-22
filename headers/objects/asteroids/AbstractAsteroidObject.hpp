#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_

#include "../../engine/base/includes.hpp"
#include "../../engine/objects/MeshObject.hpp"
#include "../../engine/objects/Collider.hpp"

struct AbstractAsteroidVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
    glm::vec4 tan;
};

class AbstractAsteroidObject : public MeshObject<AbstractAsteroidVertex>, public ICollidable {
    private:
        glm::vec3 velToUpdate;
        glm::vec3 vel;
        glm::vec3 angVel;
        float scale, scaleToUpdate;
        float minScale = 1.25f;

        void receiveDamage(float damage);
        void initializeRandom();        
    protected:
        GameObject* player;
        virtual void die();
    public:
        AbstractAsteroidObject(GameObject* player);
        virtual void Instantiate();
        void Update();
        void OnCollisionWith(Collider* other);
        virtual std::string getModelName() = 0;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_