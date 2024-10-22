#include "../../../headers/engine/graphics/ObjectVertexDescriptor.hpp"

ObjectVertexDescriptor::ObjectVertexDescriptor() {
    bindings = {};
    locations = {};
    isCompiled = false;
}

void ObjectVertexDescriptor::addBinding(uint32_t size, bool changesPerVertex) {
    uint32_t bindingIndex = (uint32_t)bindings.size();
    VertexBindingDescriptorElement binding = { bindingIndex, size, changesPerVertex ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE };
    bindings.push_back(binding);
}

void ObjectVertexDescriptor::addLocation(uint32_t bindingNumber, VkFormat format, uint32_t offset, uint32_t size, VertexDescriptorElementUsage usage) {
    uint32_t locationIndex = (uint32_t)locations.size();
    VertexDescriptorElement location = { bindingNumber, locationIndex, format, offset, size, usage };
    locations.push_back(location);
}

VertexDescriptor* ObjectVertexDescriptor::compile(BaseProject* proj) {
    if (!isCompiled) {
        compiledDescriptor = new VertexDescriptor();
        compiledDescriptor->init(proj, bindings, locations);
        isCompiled = true;
    }
    return compiledDescriptor;
}
