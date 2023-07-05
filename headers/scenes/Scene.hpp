#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "../base/includes.hpp"
#include "../objects/Collider.hpp"
#include "../objects/GameObject.hpp"

class Scene {
    public:
    protected:
        //std::vector<GameObject> activeObjects;
        std::unordered_map<uint8_t, Collider> activeColliders;
};

#endif