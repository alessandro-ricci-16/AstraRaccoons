#ifndef GAMEOBJECT_INCLUDE
#define GAMEOBJECT_INCLUDE

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../graphics/ModelComponent.hpp"

class GameObject {
    protected:
        Transform transform;
        std::optional<ModelComponent> model;
        GameObject* parentObject;
        std::vector<GameObject> children;

    public:
        GameObject();

        virtual void Instantiate();
        virtual void Start();
        virtual void Update();
        virtual void Destroy();

        void addChild(GameObject child);
        void setModel(ModelComponent model);
        
};

#endif