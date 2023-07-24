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
        float scale;
        float minScale = 1.25f;

        virtual void receiveDamage(float damage);
        void initializeRandom();        
    protected:
        float scaleToUpdate;
        GameObject* player;
        virtual void die();
        virtual void addScore(int points);
    public:
        AbstractAsteroidObject(GameObject* player);
        virtual void Instantiate();
        virtual void Update();
        void OnCollisionWith(Collider* other);
        virtual std::string getModelName() = 0;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_ABSTRACTASTEROIDOBJECT_HPP_