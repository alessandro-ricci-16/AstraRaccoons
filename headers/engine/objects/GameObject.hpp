#ifndef GAMEOBJECT_INCLUDE
#define GAMEOBJECT_INCLUDE

#include "../base/includes.hpp"
#include "Transform.hpp"
#include "../graphics/ModelComponent.hpp"

class Collider;
class Scene;

class GameObject {
    protected:
        GameObject* parentObject = nullptr;
        std::vector<GameObject*> children;
        bool acceptsGUBOs, shouldDisplayAlwaysOnTop = false;

    public:
        Transform transform;
        std::vector<Collider*> colliders;
        Scene* parentScene;

        GameObject();
        virtual ~GameObject();

        void UpdateImpl();

        virtual void Instantiate() = 0;
        virtual void Update() = 0;
        virtual void CommitUpdates(int currentimage, glm::mat4 cameraMatrix);
        virtual void Destroy() = 0;
        virtual void Cleanup() = 0;

        virtual int textureCount();
        virtual int uniformsCount();

        virtual void compile(BaseProject* proj, GlobalUniforms* guboPtr);
        virtual void addChild(GameObject* child);
        virtual void addCollider(glm::vec3 origin, float radius, uint8_t collisionLayer, uint8_t collisionMask);
        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline);

        mat4 getFullMatrix();
        bool getDisplaysAlwaysOnTop();
};

#endif