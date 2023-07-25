#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_BASEPROJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_BASEPROJECT_HPP_

#include "includes.hpp"
#include "Common.hpp"

class BaseProject {
	friend class VertexDescriptor;
	template <class Vert> friend class Model;
	friend class Texture;
	friend class Pipeline;
	friend class DescriptorSetLayout;
	friend class DescriptorSet;
	public:
		virtual void setWindowParameters() = 0;
		void run();

	protected:
		class deviceReport {
			public:
				bool swapChainAdequate;
				bool swapChainFormatSupport;
				bool swapChainPresentModeSupport;
				bool completeQueueFamily;
				bool anisotropySupport;
				bool extensionsSupported;
				std::set<std::string> requiredExtensions;
				
				void print();
		};

		uint32_t windowWidth;
		uint32_t windowHeight;
		// used to exit fullscreen
		int oldWindowWidth;
		int oldWindowHeight;
		bool windowResizable;
		std::string windowTitle;
		VkClearColorValue initialBackgroundColor;
		int uniformBlocksInPool;
		int texturesInPool;
		int setsInPool;
        const int MAX_FRAMES_IN_FLIGHT = 2;

		GLFWwindow* window;
		VkInstance instance;

		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		
		VkRenderPass renderPass;
		
		VkDescriptorPool descriptorPool;

		VkDebugUtilsMessengerEXT debugMessenger;
		
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		size_t currentFrame = 0;
		bool framebufferResized = false;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		
		
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		virtual void onWindowResize(int w, int h) = 0;
		virtual void localInit() = 0;
		virtual void pipelinesAndDescriptorSetsInit() = 0;
		virtual void populateCommandBuffer(VkCommandBuffer commandBuffer, int i) = 0;
		virtual void updateUniformBuffer(uint32_t currentImage) = 0;
		virtual void pipelinesAndDescriptorSetsCleanup() = 0;
		virtual void localCleanup() = 0;

		void initWindow();
		void initVulkan();
		void createInstance();
		std::vector<const char*> getRequiredExtensions();
		bool checkIfItHasExtension(const char *ext);
		bool checkIfItHasDeviceExtension(VkPhysicalDevice device, const char *ext);
		bool checkValidationLayerSupport();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device, deviceReport &devRep);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device, deviceReport &devRep);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSampleCountFlagBits getMaxUsableSampleCount();
		void createLogicalDevice();
		void createSwapChain();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createImageViews();
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkImageViewType type, int layerCount);
		void createRenderPass();
		void createFramebuffers();
		void createCommandPool();
		void createColorResources();
		void createDepthResources();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		bool hasStencilComponent(VkFormat format);
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, int imgCount, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags cflags, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, int layerCount);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, int layersCount);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int layerCount);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createDescriptorPool();
		void createCommandBuffers();
		void refillCommandBuffers();
		void createSyncObjects();
		void mainLoop();
		void drawFrame();
		void recreateSwapChain();
		void cleanupSwapChain();
		void cleanup();
		void RebuildPipeline();
		void setWindowIcon(GLFWwindow* window, const char* iconPath);
	
	// Public part of the base class
	public:
		// Debug commands
		void printFloat(const char *Name, float v);
		void printVec2(const char *Name, glm::vec2 v);
		void printVec3(const char *Name, glm::vec3 v);
		void printVec4(const char *Name, glm::vec4 v);
		void printMat3(const char *Name, glm::mat3 v);
		void printMat4(const char *Name, glm::mat4 v);
};

struct VertexBindingDescriptorElement {
                uint32_t binding;
                uint32_t stride;
                VkVertexInputRate inputRate;
};

enum VertexDescriptorElementUsage {
    POSITION,
    NORMAL,
    UV,
    COLOR,
    TANGENT,
    OTHER
};

struct VertexDescriptorElement {
                uint32_t binding;
                uint32_t location;
                VkFormat format;
                uint32_t offset;
                uint32_t size;
                VertexDescriptorElementUsage usage;
};

struct VertexComponent {
                bool hasIt;
                uint32_t offset;
};

struct VertexDescriptor {
                BaseProject *BP;

                VertexComponent Position;
                VertexComponent Normal;
                VertexComponent UV;
                VertexComponent Color;
                VertexComponent Tangent;

                std::vector<VertexBindingDescriptorElement> Bindings;
                std::vector<VertexDescriptorElement> Layout;

                void init(BaseProject *bp,
                          std::vector<VertexBindingDescriptorElement> B,
                          std::vector<VertexDescriptorElement> E);
                void cleanup();

                std::vector<VkVertexInputBindingDescription>
                getBindingDescription();
                std::vector<VkVertexInputAttributeDescription>
                getAttributeDescriptions();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_BASEPROJECT_HPP_
