#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "../base/includes.hpp"
#include "../objects/Collider.hpp"
#include "../objects/GameObject.hpp"
#include "../graphics/GraphicsPipeline.hpp"
#include "../objects/Camera.hpp"
#include <unordered_map>
class Scene {
    protected:
        std::vector<GameObject*> activeObjects;
        std::unordered_map<uint8_t, Collider> activeColliders;
        Camera camera;
        float aspectRatio;

    public:
        BaseProject* proj;
        GraphicsPipeline* activePipeline;
        
        Scene();
        
        //Perform initial scene setup
        virtual void Instantiate() = 0;
        //Per-frame callback (pre-draw call)
        virtual void Update() = 0;
        //Called when the scene has to be destroyed
        virtual void Destroy() = 0;

        virtual void updateAspectRatio(float aspectRatio);
        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage);
        virtual void addObject(GameObject* object);
};

#endif