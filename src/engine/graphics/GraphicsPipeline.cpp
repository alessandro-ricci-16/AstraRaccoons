#include "../../../headers/engine/graphics/GraphicsPipeline.hpp"
#include <unordered_map>

DSet::DSet() {
	bindings = {};
	setElements = {};
	isCompiled = false;
}

void DSet::addTextureBinding(Texture* tex, VkShaderStageFlags shaderStage) {
	//Add bindings for both the DSL and DS
	uint32_t bindingIndex = bindings.size();

	bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, shaderStage });
	setElements.push_back({ nullptr, { (int)bindingIndex, TEXTURE, 0, tex } });
}

void DSet::addUniformBinding(void* uniform, int uniformSize, VkShaderStageFlags shaderStage) {
	//Add bindings for both the DSL and DS
	uint32_t bindingIndex = bindings.size();

	bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStage });
	setElements.push_back({ uniform, { (int)bindingIndex, UNIFORM, uniformSize, nullptr } });
}

void DSet::compile(BaseProject* proj) {
	//Compile the DescriptorSetLayout and DescriptorSet objects
	if (!isCompiled) {
		compiledSetLayout.init(proj, bindings);
		isCompiled = true;
	}
	std::vector<DescriptorSetElement> extractedElements = {};
	for (int i = 0; i < setElements.size(); i++) {
		extractedElements.push_back(setElements.at(i).setElement);
	}

	compiledSet.init(proj, &compiledSetLayout, extractedElements);
	datasetsCleanedUp = false;
}

void DSet::map(int currentImage) {
	for (int i = 0; i < setElements.size(); i++) {
		//Map all uniforms
		DSetElement setElement = setElements.at(i);
		if (setElement.uniformPtr != nullptr) {
			//Map the uniform
			compiledSet.map(currentImage, setElement.uniformPtr, setElement.setElement.size, i);
		}
	}
}

void DSet::cleanup() {
	compiledSet.cleanup();
	datasetsCleanedUp = true;
}

void DSet::destroy() {
	if (!datasetsCleanedUp) {
		cleanup();
	}
	compiledSetLayout.cleanup();
}

void DSet::bind(VkCommandBuffer commandBuffer, Pipeline& pipeline, int setId, int currentImage) {
	//Bind
	compiledSet.bind(commandBuffer, pipeline, setId, currentImage);
}



GraphicsPipeline::GraphicsPipeline(std::string vertexShader, std::string fragmentShader) {
	vertexShaderName = vertexShader;
	fragmentShaderName = fragmentShader;
	descriptorSets = {};
	//Generate the ID from the hash of the shader files (used to recycle pipelines)
	std::string vertName, fragName;
	std::copy(vertexShader.rbegin(), vertexShader.rend(), std::back_inserter(vertName));
	std::copy(fragmentShader.rbegin(), fragmentShader.rend(), std::back_inserter(fragName));
	std::string concatNames = vertName.append(fragName);
	std::hash<std::string> hasher;
	id = hasher(concatNames);
	isInitialized = false;
}

void GraphicsPipeline::addSet() {
	DSet* dset = new DSet();
	descriptorSets.push_back(dset);
}

void GraphicsPipeline::addTextureBindingToLastSet(Texture* tex, VkShaderStageFlags shaderStage) {
	if (descriptorSets.empty()) {
		addSet();
	}

	DSet* lastDSet = descriptorSets.at(descriptorSets.size() - 1);
	lastDSet->addTextureBinding(tex, shaderStage);
}

void GraphicsPipeline::addUniformBindingToLastSet(void* uniform, int uniformSize, VkShaderStageFlags shaderStage) {
	if (descriptorSets.empty()) {
		addSet();
	}

	DSet* lastDSet = descriptorSets.at(descriptorSets.size() - 1);
	lastDSet->addUniformBinding(uniform, uniformSize, shaderStage);
}

void GraphicsPipeline::compile(BaseProject* proj, VertexDescriptor* vertexDescriptor) {
	// Compile descriptors & build the dsl pointer array
	if (!isInitialized) {
		std::vector<DescriptorSetLayout*> dslPointers;
		for (int i = 0; i < descriptorSets.size(); i++) {
			descriptorSets.at(i)->compile(proj);
			dslPointers.push_back(&(descriptorSets.at(i)->compiledSetLayout));
		}
		compiledPipeline.init(proj, vertexDescriptor, vertexShaderName, fragmentShaderName, dslPointers);
		isInitialized = true;
		//Create the pipeline
		compiledPipeline.create();
	} else {
		//Create the pipeline
		compiledPipeline.create();
		//Recompile the data sets (and not the DSLs)
		for (int i = 0; i < descriptorSets.size(); i++) {
			descriptorSets.at(i)->compile(proj);
		}
	}
}

void GraphicsPipeline::bind(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) {
	//Bind the pipeline
	if (activePipeline == nullptr || activePipeline->id != id) {
		compiledPipeline.bind(commandBuffer);
		activePipeline = this;
	}
	//Map & bind datasets
	for (int i = 0; i < descriptorSets.size(); i++) {
		descriptorSets.at(i)->bind(commandBuffer, activePipeline->compiledPipeline, i, currentImage);
	}
}

void GraphicsPipeline::commitUniforms(int currentImage) {
	//Map datasets
	for (int i = 0; i < descriptorSets.size(); i++) {
		descriptorSets.at(i)->map(currentImage);
	}
}

void GraphicsPipeline::cleanup() {
	if (isInitialized) {
		compiledPipeline.cleanup();
		for (int i = 0; i < descriptorSets.size(); i++) {
			descriptorSets.at(i)->cleanup();
		}
	}
}

void GraphicsPipeline::destroy() {
	compiledPipeline.destroy();
	isInitialized = false;
	for (int i = 0; i < descriptorSets.size(); i++) {
		descriptorSets.at(i)->destroy();
		delete descriptorSets.at(i);
	}
	descriptorSets = {};
}