#ifndef GAMEOBJECT_INCLUDE
#define GAMEOBJECT_INCLUDE

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../graphics/ModelComponent.hpp"

class GameObject {
    protected:
        GameObject* parentObject;
        std::vector<GameObject*> children;

    public:
        Transform transform;
        void* parentScene;

        GameObject();

        virtual void Instantiate() = 0;
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void CommitUpdates(int currentimage, glm::mat4 cameraMatrix);
        virtual void Destroy() = 0;

        virtual void addChild(GameObject* child);
        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix);
};

#endif