#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "../base/includes.hpp"
#include "../objects/Collider.hpp"
#include "../objects/GameObject.hpp"
#include "../graphics/GraphicsPipeline.hpp"
#include "../objects/Camera.hpp"
#include "../graphics/Uniforms.hpp"
#include <headers/objects/TextMaker.hpp>

inline auto key_selector = [](auto pair){return pair.first;};

class Game;

struct ActiveObjectElement {
    GameObject* object;
    uint64_t zIndex;
};

class Scene {
    private:
        struct ActiveObjectsCompare {
            bool operator()(const ActiveObjectElement* lhs,
                            const ActiveObjectElement* rhs) const {
                return lhs->zIndex < rhs->zIndex;
            }
        };

        std::set<ActiveObjectElement*> addedObjects;
        std::set<GameObject*> removedObjects;
        bool modifiedActiveObjects;
        bool isUpdatingScene;
        bool sceneSwitchRequested = false;
        std::string text;
        bool x, y;
        uint64_t newZIndex = 0;

    protected:
        std::set<Collider*> activeColliders; // list of all active colliders in game
        Camera* camera;
        float* aspectRatio;
        TextMaker* txt;

    public:
        std::set<ActiveObjectElement*, ActiveObjectsCompare> activeObjects;
        Game* proj;
        GraphicsPipeline* activePipeline;
        GlobalUniforms gubos;

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
        //Called when the scene is about to disappear due to scene switch
        virtual void WillDisappear() = 0;

        void UpdateImpl(int currentImage);
        void DestroyImpl();
        void CleanupImpl();
        void CompileObjects(bool addedOnly);

        int totalTextureCount();
        int totalUniformsCount();

        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage);
        virtual void addObject(GameObject* object);
        virtual void removeObject(GameObject* object);
        void applyObjectModifications();
        void requestSceneSwitch(int newScene);
        virtual void windowResize();

    private:
        void CheckCollisions();
};

#endif