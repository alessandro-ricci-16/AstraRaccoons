#include "../../../headers/engine/base/Pipeline.hpp"

void DescriptorSetLayout::init(BaseProject *bp, std::vector<DescriptorSetLayoutBinding> B) {
	BP = bp;
	
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bindings.resize(B.size());
	for(int i = 0; i < B.size(); i++) {
		bindings[i].binding = B[i].binding;
		bindings[i].descriptorType = B[i].type;
		bindings[i].descriptorCount = 1;
		bindings[i].stageFlags = B[i].flags;
		bindings[i].pImmutableSamplers = nullptr;
	}
	
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
	layoutInfo.pBindings = bindings.data();
	
	VkResult result = vkCreateDescriptorSetLayout(BP->device, &layoutInfo,
								nullptr, &descriptorSetLayout);
	if (result != VK_SUCCESS) {
		PrintVkError(result);
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void DescriptorSetLayout::cleanup() {
    	vkDestroyDescriptorSetLayout(BP->device, descriptorSetLayout, nullptr);	
}

void DescriptorSet::init(BaseProject *bp, DescriptorSetLayout *DSL,
						 std::vector<DescriptorSetElement> E) {
	BP = bp;
	
	uniformBuffers.resize(E.size());
	uniformBuffersMemory.resize(E.size());
	toFree.resize(E.size());

	for (int j = 0; j < E.size(); j++) {
		uniformBuffers[j].resize(BP->swapChainImages.size());
		uniformBuffersMemory[j].resize(BP->swapChainImages.size());
		if(E[j].type == UNIFORM) {
			for (size_t i = 0; i < BP->swapChainImages.size(); i++) {
				VkDeviceSize bufferSize = E[j].size;
				BP->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
									 	 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
									 	 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
									 	 uniformBuffers[j][i], uniformBuffersMemory[j][i]);
			}
			toFree[j] = true;
		} else {
			toFree[j] = false;
		}
	}
	
	std::vector<VkDescriptorSetLayout> layouts(BP->swapChainImages.size(),
											   DSL->descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = BP->descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(BP->swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();
	
	descriptorSets.resize(BP->swapChainImages.size());
	
	VkResult result = vkAllocateDescriptorSets(BP->device, &allocInfo,
										descriptorSets.data());
	if (result != VK_SUCCESS) {
		PrintVkError(result);
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
	
	for (size_t i = 0; i < BP->swapChainImages.size(); i++) {
		std::vector<VkWriteDescriptorSet> descriptorWrites(E.size());
		std::vector<VkDescriptorBufferInfo> bufferInfo(E.size());
		std::vector<VkDescriptorImageInfo> imageInfo(E.size());
		for (int j = 0; j < E.size(); j++) {
			if(E[j].type == UNIFORM) {
				bufferInfo[j].buffer = uniformBuffers[j][i];
				bufferInfo[j].offset = 0;
				bufferInfo[j].range = E[j].size;
				
				descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrites[j].dstSet = descriptorSets[i];
				descriptorWrites[j].dstBinding = E[j].binding;
				descriptorWrites[j].dstArrayElement = 0;
				descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrites[j].descriptorCount = 1;
				descriptorWrites[j].pBufferInfo = &bufferInfo[j];
			} else if(E[j].type == TEXTURE) {
				imageInfo[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo[j].imageView = E[j].tex->textureImageView;
				imageInfo[j].sampler = E[j].tex->textureSampler;
		
				descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrites[j].dstSet = descriptorSets[i];
				descriptorWrites[j].dstBinding = E[j].binding;
				descriptorWrites[j].dstArrayElement = 0;
				descriptorWrites[j].descriptorType =
											VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrites[j].descriptorCount = 1;
				descriptorWrites[j].pImageInfo = &imageInfo[j];
			}
		}		
		vkUpdateDescriptorSets(BP->device,
						static_cast<uint32_t>(descriptorWrites.size()),
						descriptorWrites.data(), 0, nullptr);
	}
}

void DescriptorSet::cleanup() {
	for(int j = 0; j < uniformBuffers.size(); j++) {
		if(toFree[j]) {
			for (size_t i = 0; i < BP->swapChainImages.size(); i++) {
				vkDestroyBuffer(BP->device, uniformBuffers[j][i], nullptr);
				vkFreeMemory(BP->device, uniformBuffersMemory[j][i], nullptr);
			}
		}
	}
}

void DescriptorSet::bind(VkCommandBuffer commandBuffer, Pipeline &P, int setId,
						 int currentImage) {
	vkCmdBindDescriptorSets(commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					P.pipelineLayout, setId, 1, &descriptorSets[currentImage],
					0, nullptr);
}

void DescriptorSet::map(int currentImage, void *src, int size, int slot) {
	void* data;

	vkMapMemory(BP->device, uniformBuffersMemory[slot][currentImage], 0,
						size, 0, &data);
	memcpy(data, src, size);
	vkUnmapMemory(BP->device, uniformBuffersMemory[slot][currentImage]);	
}