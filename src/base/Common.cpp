#include "../../headers/base/Common.hpp"

const std::vector<errorcode> ErrorCodes = {
    {VK_NOT_READY, "Not Ready"},
    {VK_TIMEOUT, "Timeout"},
    {VK_EVENT_SET, "Event Set"},
    {VK_EVENT_RESET, "Event Reset"},
    {VK_INCOMPLETE, "Incomplete"},
    {VK_ERROR_OUT_OF_HOST_MEMORY, "Out of Host Memory"},
    {VK_ERROR_OUT_OF_DEVICE_MEMORY, "Out of Device Memory"},
    {VK_ERROR_INITIALIZATION_FAILED, "Initialization Failed"},
    {VK_ERROR_DEVICE_LOST, "Device Lost"},
    {VK_ERROR_MEMORY_MAP_FAILED, "Memory Map Failed"},
    {VK_ERROR_LAYER_NOT_PRESENT, "Layer Not Present"},
    {VK_ERROR_EXTENSION_NOT_PRESENT, "Extension Not Present"},
    {VK_ERROR_FEATURE_NOT_PRESENT, "Feature Not Present"},
    {VK_ERROR_INCOMPATIBLE_DRIVER, "Incompatible Driver"},
    {VK_ERROR_TOO_MANY_OBJECTS, "Too Many Objects"},
    {VK_ERROR_FORMAT_NOT_SUPPORTED, "Format Not Supported"},
    {VK_ERROR_FRAGMENTED_POOL, "Fragmented Pool"},
    {VK_ERROR_SURFACE_LOST_KHR, "Surface Lost"},
    {VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "Native Window in Use"},
    {VK_SUBOPTIMAL_KHR, "Suboptimal"},
    {VK_ERROR_OUT_OF_DATE_KHR, "Error Out of Date"},
    {VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "Incompatible Display"},
    {VK_ERROR_VALIDATION_FAILED_EXT, "Valuidation Failed"},
    {VK_ERROR_INVALID_SHADER_NV, "Invalid Shader"},
    {VK_ERROR_OUT_OF_POOL_MEMORY_KHR, "Out of Pool Memory"},
    {VK_ERROR_INVALID_EXTERNAL_HANDLE, "Invalid External Handle"},

};

bool QueueFamilyIndices::isComplete() {
	return graphicsFamily.has_value() && presentFamily.has_value();
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
				vkGetInstanceProcAddr(instance,
					"vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
				vkGetInstanceProcAddr(instance,
					"vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void PrintVkError( VkResult result ) {
	const int numErrorCodes = sizeof( ErrorCodes ) / sizeof( struct errorcode );
	std::string meaning = "";
	for( int i = 0; i < numErrorCodes; i++ ) {
		if( result == ErrorCodes[i].resultCode ) {
			meaning = ErrorCodes[i].meaning;
			break;
		}
	}
	std::cout << "Error: " << result << ", " << meaning << "\n";
}


std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Failed to open: " << filename << "\n";
		throw std::runtime_error("failed to open file!");
	}
	
	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
    //std::cout << filename << " -> " << fileSize << " B\n";	 
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	 
	file.close();
	 
	return buffer;
}