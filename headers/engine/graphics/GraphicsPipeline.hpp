#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_

#include "../base/includes.hpp"
#include "../base/Pipeline.hpp"
#include "../base/BaseProject.hpp"
#include "../base/Texture.hpp"
#include "Uniforms.hpp"

class DSet {
    private:
        struct DSetElement {
            void* uniformPtr = nullptr;
            DescriptorSetElement setElement;
        };

        std::vector<DescriptorSetLayoutBinding> bindings;
        std::vector<DSetElement> setElements;

    public:
        DescriptorSet compiledSet;
        DescriptorSetLayout compiledSetLayout;
        
        DSet();

        void addTextureBinding(Texture* tex, VkShaderStageFlags shaderStage);
        void addUniformBinding(void* uniform, int uniformSize, VkShaderStageFlags shaderStage);
        void compile(BaseProject* proj);
        void map(int currentImage);
        void bind(VkCommandBuffer commandBuffer, Pipeline &pipeline, int setId, int currentImage);
};

class GraphicsPipeline {
    private:
        std::string vertexShaderName, fragmentShaderName;
        std::vector<DSet*> descriptorSets;
        Pipeline compiledPipeline;
        bool isInitialized;
    protected:
        uint32_t id;

    public:
        GraphicsPipeline() = default;
        GraphicsPipeline(std::string vertexShader, std::string fragmentShader);

        void addSet();
        void addTextureBindingToLastSet(Texture* tex, VkShaderStageFlags shaderStage);
        void addUniformBindingToLastSet(void* uniform, int uniformSize, VkShaderStageFlags shaderStage);
        void compile(BaseProject* proj, VertexDescriptor* vertexDescriptor);
        void cleanup();
        void destroy();

        void commitUniforms(int currentImage);
        void bind(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_