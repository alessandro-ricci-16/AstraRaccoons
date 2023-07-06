#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_TEXTURE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_TEXTURE_HPP_

#include "includes.hpp"
#include "BaseProject.hpp"

struct Texture {
	BaseProject *BP;
	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	int imgs;
	static const int maxImgs = 6;
	
	void createTextureImage(const char *const files[], VkFormat Fmt = VK_FORMAT_R8G8B8A8_SRGB);
	void createTextureImageView(VkFormat Fmt = VK_FORMAT_R8G8B8A8_SRGB);
	void createTextureSampler(VkFilter magFilter = VK_FILTER_LINEAR,
							 VkFilter minFilter = VK_FILTER_LINEAR,
							 VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
							 VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
							 VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
							 VkBool32 anisotropyEnable = VK_TRUE,
							 float maxAnisotropy = 16,
							 float maxLod = -1
							);

    void init(BaseProject *bp, const char *file, VkFormat Fmt = VK_FORMAT_R8G8B8A8_SRGB, bool initSampler = true);
    void initCubic(BaseProject *bp, const char * files[6]);
	void cleanup();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_TEXTURE_HPP_