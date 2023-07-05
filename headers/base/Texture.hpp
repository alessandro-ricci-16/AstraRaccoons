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
	
	void createTextureImage(const char *const files[], VkFormat Fmt);
	void createTextureImageView(VkFormat Fmt);
	void createTextureSampler(VkFilter magFilter,
							 VkFilter minFilter,
							 VkSamplerAddressMode addressModeU,
							 VkSamplerAddressMode addressModeV,
							 VkSamplerMipmapMode mipmapMode,
							 VkBool32 anisotropyEnable,
							 float maxAnisotropy,
							 float maxLod
							);

	void init(BaseProject *bp, const char * file, VkFormat Fmt, bool initSampler);
	void initCubic(BaseProject *bp, const char * files[6]);
	void cleanup();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_TEXTURE_HPP_