#ifndef GAMEOBJECT_INCLUDE
#define GAMEOBJECT_INCLUDE

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../graphics/ModelComponent.hpp"

class Collider;

class GameObject {
    protected:
        GameObject* parentObject;
        std::vector<GameObject*> children;

    public:
        Transform transform;
        Collider* collider;
        void* parentScene;

        GameObject();
        virtual ~GameObject();

        virtual void Instantiate() = 0;
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void CommitUpdates(int currentimage, glm::mat4 cameraMatrix);
        virtual void Destroy() = 0;
        virtual void Cleanup() = 0;

        virtual void compile(BaseProject* proj, GlobalUniforms* guboPtr);
        virtual void addChild(GameObject* child);
        virtual void setCollider(float radius, uint8_t collisionLayer, uint8_t collisionMask);
        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix);
};

class ICollidable {
    public:
        virtual void OnCollisionWith(GameObject* other) = 0;
};

#endif