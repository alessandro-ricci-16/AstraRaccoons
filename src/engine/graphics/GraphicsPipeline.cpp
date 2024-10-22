#include "../../../headers/engine/graphics/GraphicsPipeline.hpp"
#include <unordered_map>

DSet* GraphicsPipeline::guboSet = nullptr;

DSet::DSet() {
	bindings = {};
	setElements = {};
	isCompiled = false;
	datasetsCleanedUp = true;
}

void DSet::addTextureBinding(Texture* tex, VkShaderStageFlags shaderStage) {
	//Add bindings for both the DSL and DS
	uint32_t bindingIndex = (uint32_t)bindings.size();

	bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, shaderStage });
	setElements.push_back({ nullptr, { (int)bindingIndex, TEXTURE, 0, tex } });
}

void DSet::addUniformBinding(void* uniform, int uniformSize, VkShaderStageFlags shaderStage) {
	//Add bindings for both the DSL and DS
	uint32_t bindingIndex = (uint32_t)bindings.size();

	bindings.push_back({ bindingIndex, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, shaderStage });
	setElements.push_back({ uniform, { (int)bindingIndex, UNIFORM, uniformSize, nullptr } });
}

void DSet::compile(BaseProject* proj) {
	//Compile the DescriptorSetLayout and DescriptorSet objects
	if (!isCompiled) {
		compiledSetLayout.init(proj, bindings);
		isCompiled = true;
	}
	
	if (datasetsCleanedUp) {
		std::vector<DescriptorSetElement> extractedElements = {};
		for (int i = 0; i < setElements.size(); i++) {
			extractedElements.push_back(setElements.at(i).setElement);
		}

		compiledSet.init(proj, &compiledSetLayout, extractedElements);
	}
	datasetsCleanedUp = false;
}

void DSet::map(int currentImage) {
	if (!isCompiled || datasetsCleanedUp) { return; }

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
	if (!datasetsCleanedUp) {
		compiledSet.cleanup();
	}
	datasetsCleanedUp = true;
}

void DSet::destroy() {
	if (!datasetsCleanedUp) {
		cleanup();
	}
	if (isCompiled) {
		compiledSetLayout.cleanup();
	}
	isCompiled = false;
	datasetsCleanedUp = true;
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
	id = (uint32_t)hasher(concatNames);
	isInitialized = false;
	isDestroyed = true;
}

void GraphicsPipeline::addSet(bool isGUBOSet) {
	DSet* dset;
	if (isGUBOSet) {
		if (GraphicsPipeline::guboSet == nullptr) {
			dset = new DSet();
			isUsingCommonGUBOs = false;
			GraphicsPipeline::guboSet = dset;
		} else {
			dset = GraphicsPipeline::guboSet;
			isUsingCommonGUBOs = true;
		}
	} else {
		dset = new DSet();
		isUsingCommonGUBOs = false;
	}
	descriptorSets.push_back(dset);
}

void GraphicsPipeline::addTextureBindingToLastSet(Texture* tex, VkShaderStageFlags shaderStage) {
	if (descriptorSets.empty()) {
		addSet(false);
	}
	if (isUsingCommonGUBOs) { return; }

	DSet* lastDSet = descriptorSets.at(descriptorSets.size() - 1);
	lastDSet->addTextureBinding(tex, shaderStage);
}

void GraphicsPipeline::addUniformBindingToLastSet(void* uniform, int uniformSize, VkShaderStageFlags shaderStage) {
	if (descriptorSets.empty()) {
		addSet(false);
	}
	if (isUsingCommonGUBOs) { return; }

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
		compiledPipeline.transp = isTransparent;
		if (!backfaceCullingOn) {
			compiledPipeline.CM = VK_CULL_MODE_NONE;
		}
		isInitialized = true;
		//Create the pipeline
		compiledPipeline.create();
	} else {
		compiledPipeline.transp = isTransparent;
		if (!backfaceCullingOn) {
			compiledPipeline.CM = VK_CULL_MODE_NONE;
		}
		//Create the pipeline
		compiledPipeline.create();
		//Recompile the data sets (and not the DSLs)
		for (int i = 0; i < descriptorSets.size(); i++) {
			descriptorSets.at(i)->compile(proj);
		}
	}
	isDestroyed = false;
}

void GraphicsPipeline::bind(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) {
	if (isDestroyed) {
		std::runtime_error("BINDING DESTROYED PIPELINE!");
	}
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
			if (descriptorSets[i] != GraphicsPipeline::guboSet) {
				descriptorSets.at(i)->cleanup();
			}
		}
	}
}

void GraphicsPipeline::destroy() {
	compiledPipeline.destroy();
	isInitialized = false;
	for (int i = 0; i < descriptorSets.size(); i++) {
		if (descriptorSets[i] != GraphicsPipeline::guboSet) {
			descriptorSets.at(i)->destroy();
			delete descriptorSets[i];
		}
	}
	descriptorSets.clear();
	isDestroyed = true;
}

void GraphicsPipeline::cleanupGUBOs() {
	if (GraphicsPipeline::guboSet != nullptr) {
		GraphicsPipeline::guboSet->cleanup();
	}
}

void GraphicsPipeline::destroyGUBOs() {
	if (GraphicsPipeline::guboSet != nullptr) {
		GraphicsPipeline::guboSet->destroy();
		delete GraphicsPipeline::guboSet;
		GraphicsPipeline::guboSet = nullptr;
	}
}
