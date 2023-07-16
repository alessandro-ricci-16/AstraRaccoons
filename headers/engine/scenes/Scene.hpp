#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "../base/includes.hpp"
#include "../objects/Collider.hpp"
#include "../objects/GameObject.hpp"
#include "../graphics/GraphicsPipeline.hpp"
#include "../objects/Camera.hpp"
#include "../graphics/Uniforms.hpp"
#include <unordered_map>

inline auto key_selector = [](auto pair){return pair.first;};

class Scene {
    private:
        std::vector<GameObject*> removedObjects;
        bool modifiedActiveObjects;

    protected:
        std::unordered_map<uint8_t, std::vector<Collider*>> activeColliders; //Maps the collision mask to the colliders with that mask
        Camera camera;
        float* aspectRatio;
        GlobalUniforms gubos;

    public:
        std::vector<GameObject*> activeObjects;
        BaseProject* proj;
        GraphicsPipeline* activePipeline;
        
        Scene(float* ar);
        virtual ~Scene();
        
        //Perform initial scene setup
        virtual void Instantiate() = 0;
        //Per-frame callback (pre-draw call)
        virtual void Update() = 0;
        // Called when the scene has to be cleaned up
        virtual void Cleanup() = 0;
        //Called when the scene has to be destroyed
        virtual void Destroy() = 0;

        void UpdateImpl(int currentImage);
        void DestroyImpl();
        void CleanupImpl();
        void CompileObjects();

        int totalTextureCount();
        int totalUniformsCount();

        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage);
        virtual void addObject(GameObject* object);
        virtual void removeObject(GameObject* object);

    private:
        void CheckCollisions();
};

#endif