#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_PIPELINE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_PIPELINE_HPP_

#include "includes.hpp"
#include "BaseProject.hpp"
#include "Texture.hpp"

enum DescriptorSetElementType { UNIFORM, TEXTURE };

struct Pipeline;

struct DescriptorSetLayoutBinding {
    uint32_t binding;
    VkDescriptorType type;
    VkShaderStageFlags flags;
};

struct DescriptorSetLayout {
    BaseProject *BP;
    VkDescriptorSetLayout descriptorSetLayout;

    void init(BaseProject *bp, std::vector<DescriptorSetLayoutBinding> B);
    void cleanup();
};

struct DescriptorSetElement {
    int binding;
    DescriptorSetElementType type;
    int size;
    Texture *tex;
};

struct DescriptorSet {
    BaseProject *BP;

    std::vector<std::vector<VkBuffer>> uniformBuffers;
    std::vector<std::vector<VkDeviceMemory>> uniformBuffersMemory;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<bool> toFree;

    void init(BaseProject *bp, DescriptorSetLayout *L,
              std::vector<DescriptorSetElement> E);
    void cleanup();
    void bind(VkCommandBuffer commandBuffer, Pipeline &P, int setId, int currentImage);
    void map(int currentImage, void *src, int size, int slot);
};

struct Pipeline {
	BaseProject *BP;
	VkPipeline graphicsPipeline;
  	VkPipelineLayout pipelineLayout;
 
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	std::vector<DescriptorSetLayout *> D;	
	
	VkCompareOp compareOp;
	VkPolygonMode polyModel;
 	VkCullModeFlagBits CM;
 	bool transp;
	
	VertexDescriptor *VD;
  	
  	void init(BaseProject *bp, VertexDescriptor *vd,
			  const std::string& VertShader, const std::string& FragShader,
  			  std::vector<DescriptorSetLayout *> D);
  	void setAdvancedFeatures(VkCompareOp _compareOp, VkPolygonMode _polyModel,
 						VkCullModeFlagBits _CM, bool _transp);
  	void create();
  	void destroy();
  	void bind(VkCommandBuffer commandBuffer);
  	
  	VkShaderModule createShaderModule(const std::vector<char>& code);
	void cleanup();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_PIPELINE_HPP_