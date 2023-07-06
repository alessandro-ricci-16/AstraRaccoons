#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_

#include "../base/includes.hpp"
#include "../base/Pipeline.hpp"
#include "../base/BaseProject.hpp"
#include "../base/Texture.hpp"
#include "Uniforms.hpp"

class DSet {
    private:
        std::vector<DescriptorSetLayoutBinding> bindings;
        std::vector<DescriptorSetElement> setElements;

    public:
        DescriptorSet compiledSet;
        DescriptorSetLayout compiledSetLayout;
        
        DSet();

        void addTextureBinding(Texture* tex, VkShaderStageFlags shaderStage);
        void addUniformBinding(int uniformSize, VkShaderStageFlags shaderStage);
        void compile(BaseProject* proj);
};

class GraphicsPipeline {
    private:
        std::string vertexShaderName, fragmentShaderName;
        std::vector<DSet> descriptorSets;
        Pipeline compiledPipeline;

    protected:
        uint32_t id;

    public:
        GraphicsPipeline(std::string vertexShader, std::string fragmentShader);

        void addSet();
        void addTextureBindingToLastSet(Texture* tex, VkShaderStageFlags shaderStage);
        void addUniformBindingToLastSet(int uniformSize, VkShaderStageFlags shaderStage);
        void compile(BaseProject* proj, VertexDescriptor* vertexDescriptor);
        void cleanup();
        void destroy();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_GRAPHICSPIPELINE_HPP_