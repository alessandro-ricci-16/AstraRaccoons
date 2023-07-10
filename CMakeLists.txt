cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the paths to Vulkan and GLFW
set(VKPATH "C:/VulkanSDK/1.3.246.0/Lib")
set(GLFWPATH "C:/VulkanSDK/glfw-3.3.8.bin.WIN64/lib-vc2022")
set(VKHEADPATH "C:/VulkanSDK/1.3.246.0/Include")
set(GLFWHEADPATH "C:/VulkanSDK/glfw-3.3.8.bin.WIN64/include")

# Set the include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/headers
    ${VKHEADPATH}
    ${GLFWHEADPATH}
)

# Find Vulkan and GLFW libraries
find_library(VULKAN_LIBRARY vulkan-1 HINTS ${VKPATH})
find_library(GLFW_LIBRARY glfw3 HINTS ${GLFWPATH})

# Add the Vulkan and GLFW libraries
link_libraries(
    ${VULKAN_LIBRARY}
    ${GLFW_LIBRARY}
)

# Set the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
