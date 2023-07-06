#include "../../../headers/engine/graphics/GraphicsPipeline.hpp"
#include <unordered_map>

DSet::DSet() {
    bindings = {};
    setElements = {};
}

void DSet::addTextureBinding(Texture* tex, VkShaderStageFlags shaderStage) {
    //Add bindings for both the DSL and DS
    uint32_t bindingIndex = bindings.size();

    bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, shaderStage });
    setElements.push_back({ (int)bindingIndex, TEXTURE, 0, tex });
}

void DSet::addUniformBinding(int uniformSize, VkShaderStageFlags shaderStage) {
    //Add bindings for both the DSL and DS
    uint32_t bindingIndex = bindings.size();

    bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStage });
    setElements.push_back({ (int)bindingIndex, UNIFORM, uniformSize, nullptr });
}

void DSet::compile(BaseProject* proj) {
    //Compile the DescriptorSetLayout and DescriptorSet objects
    compiledSetLayout.init(proj, bindings);
    compiledSet.init(proj, &compiledSetLayout, setElements);
}



GraphicsPipeline::GraphicsPipeline(std::string vertexShader, std::string fragmentShader) {
    vertexShaderName = vertexShader;
    fragmentShaderName = fragmentShader;
    descriptorSets = {};
    //Generate the ID from the hash of the shader files (used to recycle pipelines)
    std::string concatNames = vertexShaderName.append(fragmentShaderName);
    std::hash<std::string> hasher;
    id = hasher(concatNames);
}

void GraphicsPipeline::addSet() {
    descriptorSets.push_back(DSet());
}

void GraphicsPipeline::addTextureBindingToLastSet(Texture* tex, VkShaderStageFlags shaderStage) {
    if (descriptorSets.empty()) {
        addSet();
    }

    DSet lastDSet = descriptorSets.at(descriptorSets.size() - 1);
    lastDSet.addTextureBinding(tex, shaderStage);
}

void GraphicsPipeline::addUniformBindingToLastSet(int uniformSize, VkShaderStageFlags shaderStage) {
    if (descriptorSets.empty()) {
        addSet();
    }

    DSet lastDSet = descriptorSets.at(descriptorSets.size() - 1);
    lastDSet.addUniformBinding(uniformSize, shaderStage);
}

void GraphicsPipeline::compile(BaseProject* proj, VertexDescriptor* vertexDescriptor) {
    // Compile descriptors & build the dsl pointer array
    std::vector<DescriptorSetLayout*> dslPointers;
    for (int i = 0; i < descriptorSets.size(); i++) {
        descriptorSets.at(i).compile(proj);
        dslPointers.push_back(&descriptorSets.at(i).compiledSetLayout);
    }
    //Create the pipeline
    compiledPipeline.init(proj, vertexDescriptor, vertexShaderName, fragmentShaderName, dslPointers);
}

void GraphicsPipeline::cleanup() {
    compiledPipeline.cleanup();
    for (int i = 0; i < descriptorSets.size(); i++) {
        descriptorSets.at(i).compiledSet.cleanup();
        descriptorSets.at(i).compiledSetLayout.cleanup();
    }
}

void GraphicsPipeline::destroy() {
    compiledPipeline.destroy();
    for (int i = 0; i < descriptorSets.size(); i++) {
        descriptorSets.at(i).compiledSet.cleanup();
        descriptorSets.at(i).compiledSetLayout.cleanup();
    }
}