#include "../../../headers/engine/base/Texture.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../../headers/engine/helpers/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../headers/engine/helpers/stb_image.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "../../../headers/engine/helpers/tiny_gltf.h"

void Texture::createTextureImage(const char *const files[], VkFormat Fmt) {
	int texWidth, texHeight, texChannels;
	int curWidth = -1, curHeight = -1, curChannels = -1;
	stbi_uc* pixels[maxImgs];
	
	for(int i = 0; i < imgs; i++) {
	 	pixels[i] = stbi_load(files[i], &texWidth, &texHeight,
						&texChannels, STBI_rgb_alpha);
		if (!pixels[i]) {
			std::cout << "Not found: " << files[i] << "\n";
			throw std::runtime_error("failed to load texture image!");
		}
		std::cout << "[" << i << "]" << files[i] << " -> size: " << texWidth
				  << "x" << texHeight << ", ch: " << texChannels <<"\n";
				  
		if(i == 0) {
			curWidth = texWidth;
			curHeight = texHeight;
			curChannels = texChannels;
		} else {
			if((curWidth != texWidth) ||
			   (curHeight != texHeight) ||
			   (curChannels != texChannels)) {
				throw std::runtime_error("multi texture images must be all of the same size!");
			}
		}
	}
	
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VkDeviceSize totalImageSize = texWidth * texHeight * 4 * imgs;
	mipLevels = static_cast<uint32_t>(std::floor(
					std::log2(std::max(texWidth, texHeight)))) + 1;
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	 
	BP->createBuffer(totalImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	  						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	  						VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	  						stagingBuffer, stagingBufferMemory);
	void* data;
	vkMapMemory(BP->device, stagingBufferMemory, 0, totalImageSize, 0, &data);
	for(int i = 0; i < imgs; i++) {
		memcpy(static_cast<char *>(data) + imageSize * i, pixels[i], static_cast<size_t>(imageSize));
		stbi_image_free(pixels[i]);
	}
	vkUnmapMemory(BP->device, stagingBufferMemory);
	
	
	BP->createImage(texWidth, texHeight, mipLevels, imgs, VK_SAMPLE_COUNT_1_BIT, Fmt,
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				imgs == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage,
				textureImageMemory);
				
	BP->transitionImageLayout(textureImage, Fmt,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels, imgs);
	BP->copyBufferToImage(stagingBuffer, textureImage,
			static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), imgs);

	BP->generateMipmaps(textureImage, Fmt,
					texWidth, texHeight, mipLevels, imgs);

	vkDestroyBuffer(BP->device, stagingBuffer, nullptr);
	vkFreeMemory(BP->device, stagingBufferMemory, nullptr);
}

void Texture::createTextureImageView(VkFormat Fmt) {
	textureImageView = BP->createImageView(textureImage,
									   Fmt,
									   VK_IMAGE_ASPECT_COLOR_BIT,
									   mipLevels,
									   imgs == 6 ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D,
									   imgs);
}
	
void Texture::createTextureSampler(
							 VkFilter magFilter,
							 VkFilter minFilter,
							 VkSamplerAddressMode addressModeU,
							 VkSamplerAddressMode addressModeV,
							 VkSamplerMipmapMode mipmapMode,
							 VkBool32 anisotropyEnable,
							 float maxAnisotropy,
							 float maxLod) {
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = magFilter;
	samplerInfo.minFilter = minFilter;
	samplerInfo.addressModeU = addressModeU;
	samplerInfo.addressModeV = addressModeV;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = anisotropyEnable;
	samplerInfo.maxAnisotropy = maxAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = mipmapMode;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = ((maxLod == -1) ? static_cast<float>(mipLevels) : maxLod);
	
	VkResult result = vkCreateSampler(BP->device, &samplerInfo, nullptr,
									  &textureSampler);
	if (result != VK_SUCCESS) {
	 	PrintVkError(result);
	 	throw std::runtime_error("failed to create texture sampler!");
	}
}

void Texture::init(BaseProject *bp, const char *  file, VkFormat Fmt, bool initSampler) {
	const char *files[1] = {file};
	BP = bp;
	imgs = 1;
	createTextureImage(files, Fmt);
	createTextureImageView(Fmt);
	if(initSampler) {
		createTextureSampler();
	}
}

void Texture::initCubic(BaseProject *bp, const char * files[6]) {
	BP = bp;
	imgs = 6;
	createTextureImage(files);
	createTextureImageView();
	createTextureSampler();
}

void Texture::cleanup() {
   	vkDestroySampler(BP->device, textureSampler, nullptr);
   	vkDestroyImageView(BP->device, textureImageView, nullptr);
	vkDestroyImage(BP->device, textureImage, nullptr);
	vkFreeMemory(BP->device, textureImageMemory, nullptr);
}