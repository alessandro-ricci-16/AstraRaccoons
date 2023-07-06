#include "../../../headers/engine/base/BaseProject.hpp"

void VertexDescriptor::init(BaseProject *bp, std::vector<VertexBindingDescriptorElement> B, std::vector<VertexDescriptorElement> E) {
	BP = bp;
	Bindings = B;
	Layout = E;
	
	Position.hasIt = false; Position.offset = 0;
	Normal.hasIt = false; Normal.offset = 0;
	UV.hasIt = false; UV.offset = 0;
	Color.hasIt = false; Color.offset = 0;
	Tangent.hasIt = false; Tangent.offset = 0;
	
	if(B.size() == 1) {	// for now, read models only with every vertex information in a single binding
		for(int i = 0; i < E.size(); i++) {
			switch(E[i].usage) {
			  case VertexDescriptorElementUsage::POSITION:
			    if(E[i].format == VK_FORMAT_R32G32B32_SFLOAT) {
				  if(E[i].size == sizeof(glm::vec3)) {
					Position.hasIt = true;
					Position.offset = E[i].offset;
				  } else {
					std::cout << "Vertex Position - wrong size\n";
				  }
				} else {
				  std::cout << "Vertex Position - wrong format\n";
				}
			    break;
			  case VertexDescriptorElementUsage::NORMAL:
			    if(E[i].format == VK_FORMAT_R32G32B32_SFLOAT) {
				  if(E[i].size == sizeof(glm::vec3)) {
					Normal.hasIt = true;
					Normal.offset = E[i].offset;
				  } else {
					std::cout << "Vertex Normal - wrong size\n";
				  }
				} else {
				  std::cout << "Vertex Normal - wrong format\n";
				}
			    break;
			  case VertexDescriptorElementUsage::UV:
			    if(E[i].format == VK_FORMAT_R32G32_SFLOAT) {
				  if(E[i].size == sizeof(glm::vec2)) {
					UV.hasIt = true;
					UV.offset = E[i].offset;
				  } else {
					std::cout << "Vertex UV - wrong size\n";
				  }
				} else {
				  std::cout << "Vertex UV - wrong format\n";
				}
			    break;
			  case VertexDescriptorElementUsage::COLOR:
			    if(E[i].format == VK_FORMAT_R32G32B32_SFLOAT) {
				  if(E[i].size == sizeof(glm::vec3)) {
					Color.hasIt = true;
					Color.offset = E[i].offset;
				  } else {
					std::cout << "Vertex Color - wrong size\n";
				  }
				} else {
				  std::cout << "Vertex Color - wrong format\n";
				}
			    break;
			  case VertexDescriptorElementUsage::TANGENT:
			    if(E[i].format == VK_FORMAT_R32G32B32A32_SFLOAT) {
				  if(E[i].size == sizeof(glm::vec4)) {
					Tangent.hasIt = true;
					Tangent.offset = E[i].offset;
				  } else {
					std::cout << "Vertex Tangent - wrong size\n";
				  }
				} else {
				  std::cout << "Vertex Tangent - wrong format\n";
				}
			    break;
			  default:
			    break;
			}
		}
	} else {
		throw std::runtime_error("Vertex format with more than one binding is not supported yet\n");
	}
}

void VertexDescriptor::cleanup() {
}

std::vector<VkVertexInputBindingDescription> VertexDescriptor::getBindingDescription() {
	std::vector<VkVertexInputBindingDescription>bindingDescription{};
	bindingDescription.resize(Bindings.size());
	for(int i = 0; i < Bindings.size(); i++) {
		bindingDescription[i].binding = Bindings[i].binding;
		bindingDescription[i].stride = Bindings[i].stride;
		bindingDescription[i].inputRate = Bindings[i].inputRate;
	}
	return bindingDescription;
}
	
std::vector<VkVertexInputAttributeDescription> VertexDescriptor::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};	
	attributeDescriptions.resize(Layout.size());
	for(int i = 0; i < Layout.size(); i++) {
		attributeDescriptions[i].binding = Layout[i].binding;
		attributeDescriptions[i].location = Layout[i].location;
		attributeDescriptions[i].format = Layout[i].format;
		attributeDescriptions[i].offset = Layout[i].offset;
	}
					
	return attributeDescriptions;
}