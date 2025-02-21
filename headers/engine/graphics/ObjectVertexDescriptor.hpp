#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_OBJECTVERTEXDESCRIPTOR_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_OBJECTVERTEXDESCRIPTOR_HPP_

#include "../base/includes.hpp"
#include "../base/BaseProject.hpp"

struct Vertex {
    glm::vec3 pos;
};

class ObjectVertexDescriptor {
    private:
        VertexDescriptor* compiledDescriptor;
        bool isCompiled;

    protected:
        std::vector<VertexBindingDescriptorElement> bindings;
        std::vector<VertexDescriptorElement> locations;

    public:
        ObjectVertexDescriptor();

        void addBinding(uint32_t size, bool changesPerVertex);
        void addLocation(uint32_t bindingNumber, VkFormat format, uint32_t offset, uint32_t size, VertexDescriptorElementUsage usage);
        VertexDescriptor* compile(BaseProject* proj);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_OBJECTVERTEXDESCRIPTOR_HPP_