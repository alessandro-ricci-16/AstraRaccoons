#ifndef GAME_DEFINED
#define GAME_DEFINED

#include "engine/base/BaseProject.hpp"
#include "scenes/MainScene.hpp"
#include <headers/objects/TextMaker.hpp>

class Game: public BaseProject {
    private:
        //The window aspect ratio
        float Ar;
        //The list of scenes managed by the game
        std::vector<Scene*> managedScenes;
        //The currently active scene index
        int activeScene, nextSceneToSwitch;
        bool sceneSwitchRequested = false;
        bool fullscreen, fullScreenClicked = false;
        void toggleFullscreen();
        void performSceneSwitchIfRequested();
    
    public:
        Game();
        void setWindowParameters();
            
        // What to do when the window changes size
        void onWindowResize(int w, int h);
            
        // Here you load and setup all your Vulkan Models and Texutures.
        // Here you also create your Descriptor set layouts and load the shaders for the pipelines
        void localInit();

        // Here you create your pipelines and Descriptor Sets!
        void pipelinesAndDescriptorSetsInit();

        // Here you destroy your pipelines and Descriptor Sets!
        // All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
        void pipelinesAndDescriptorSetsCleanup();

        // Here you destroy all the Models, Texture and Desc. Set Layouts you created!
        // All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
        // You also have to destroy the pipelines: since they need to be rebuilt, they have two different
        // methods: .cleanup() recreates them, while .destroy() delete them completely
        void localCleanup();
            
        // Here it is the creation of the command buffer:
        // You send to the GPU all the objects you want to draw,
        // with their buffers and textures
        void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage);

        // Here is where you update the uniforms.
        // Very likely this will be where you will be writing the logic of your application.
        void updateUniformBuffer(uint32_t currentImage);
        void recreateVulkanSwapChain(bool commandBufferOnly = false);
        bool requestSwitchToScene(int sceneID);

        uint32_t getWidth();
        uint32_t getHeight();
        float getAr();
};

#endif /* GAME_DEFINED */