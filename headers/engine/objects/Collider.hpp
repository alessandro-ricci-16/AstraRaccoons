#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_

#include "../base/includes.hpp"
#include "GameObject.hpp"

class Collider {
    private:
        glm::vec3 origin;
        float radius;
        uint8_t collisionLayer, collisionMask;
        GameObject* parent;

    public:
        Collider(GameObject* parent, float radius, uint8_t collisionLayer, uint8_t collisionMask, glm::vec3 origin = glm::vec3(0));
        bool compensateCompenetration = true;
        uint8_t getCollisionLayer();
        uint8_t getCollisionMask();
        GameObject* getParent();
        float getRadius();
        void setRadius(float radius);
        bool checkCollisionWith(Collider* collider2);
        void compensateCompenetrations(Collider* collider2);
};

class ICollidable {
    public:
        virtual void OnCollisionWith(Collider* other) = 0;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_