#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "../engine/base/includes.hpp"
#include "../engine/objects/Collider.hpp"
#include "../engine/objects/GameObject.hpp"

class Scene {
    public:
    protected:
        //std::vector<GameObject> activeObjects;
        std::unordered_map<uint8_t, Collider> activeColliders;
};

#endif