#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_

#include "../base/includes.hpp"
#include "GameObject.hpp"

class Collider {
    private:
        float radius;
        uint8_t collisionLayer, collisionMask;
        GameObject* parent;

    public:
        Collider(float radius, uint8_t collisionLayer, uint8_t collisionMask);

        uint8_t getCollisionLayer();
        uint8_t getCollisionMask();
        void setParent(GameObject* object);
        bool checkCollisionWith(Collider* collider2);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_COLLIDER_HPP_