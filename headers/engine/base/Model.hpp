#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_MODEL_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_MODEL_HPP_

#include "includes.hpp"
#include "BaseProject.hpp"

enum ModelType {OBJ, GLTF, MGCG};

template <class Vert>
class Model {
	BaseProject *BP;
	
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VertexDescriptor *VD;

	public:
	std::vector<Vert> vertices{};
	std::vector<uint32_t> indices{};
	void loadModelOBJ(std::string file);
	void loadModelGLTF(std::string file, bool encoded);
	void createIndexBuffer();
	void createVertexBuffer();

	void init(BaseProject *bp, VertexDescriptor *VD, std::string file, ModelType MT);
	void initMesh(BaseProject *bp, VertexDescriptor *VD);
	void cleanup();
  	void bind(VkCommandBuffer commandBuffer);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_BASE_MODEL_HPP_

#ifdef MODEL_IMPLEMENTATION_
#undef MODEL_IMPLEMENTATION_

#define TINYOBJLOADER_IMPLEMENTATION
#include "../helpers/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../helpers/stb_image.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "../helpers/tiny_gltf.h"

#define SINFL_IMPLEMENTATION
#include "../helpers/sinfl.h"

template <class Vert>
void Model<Vert>::loadModelOBJ(std::string file) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        std::cout << "Loading : " << file << "[OBJ]\n";
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              file.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::cout << "Building\n";
        //	std::cout << "Position " << VD->Position.hasIt << "," <<
        //VD->Position.offset << "\n"; 	std::cout << "UV " << VD->UV.hasIt << ","
        //<< VD->UV.offset << "\n"; 	std::cout << "Normal " << VD->Normal.hasIt
        //<< "," << VD->Normal.offset << "\n";
        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                Vert vertex{};
                glm::vec3 pos = {attrib.vertices[3 * index.vertex_index + 0],
                                 attrib.vertices[3 * index.vertex_index + 1],
                                 attrib.vertices[3 * index.vertex_index + 2]};
                if (VD->Position.hasIt) {
                    glm::vec3 *o =
                        (glm::vec3 *)((char *)(&vertex) + VD->Position.offset);
                    *o = pos;
                }

                glm::vec3 color = {attrib.colors[3 * index.vertex_index + 0],
                                   attrib.colors[3 * index.vertex_index + 1],
                                   attrib.colors[3 * index.vertex_index + 2]};
                if (VD->Color.hasIt) {
                    glm::vec3 *o =
                        (glm::vec3 *)((char *)(&vertex) + VD->Color.offset);
                    *o = color;
                }

                glm::vec2 texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]};
                if (VD->UV.hasIt) {
                    glm::vec2 *o =
                        (glm::vec2 *)((char *)(&vertex) + VD->UV.offset);
                    *o = texCoord;
                }

                glm::vec3 norm = {attrib.normals[3 * index.normal_index + 0],
                                  attrib.normals[3 * index.normal_index + 1],
                                  attrib.normals[3 * index.normal_index + 2]};
                if (VD->Normal.hasIt) {
                    glm::vec3 *o =
                        (glm::vec3 *)((char *)(&vertex) + VD->Normal.offset);
                    *o = norm;
                }

                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
        }
        std::cout << "[OBJ] Vertices: " << vertices.size() << "\n";
        std::cout << "Indices: " << indices.size() << "\n";
}

template <class Vert>
void Model<Vert>::loadModelGLTF(std::string file, bool encoded) {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string warn, err;

        std::cout << "Loading : " << file << (encoded ? "[MGCG]" : "[GLTF]")
                  << "\n";
        if (encoded) {
            auto modelString = readFile(file);

            const std::vector<unsigned char> key = plusaes::key_from_string(
                &"CG2023SkelKey128");  // 16-char = 128-bit
            const unsigned char iv[16] = {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            };

            // decrypt
            unsigned long padded_size = 0;
            std::vector<unsigned char> decrypted(modelString.size());

            plusaes::decrypt_cbc((unsigned char *)modelString.data(),
                                 modelString.size(), &key[0], key.size(), &iv,
                                 &decrypted[0], decrypted.size(), &padded_size);

            int size = 0;
            void *decomp;

            sscanf(reinterpret_cast<char *const>(&decrypted[0]), "%d", &size);
            // std::cout << decrypted.size() << ", decomp: " << size << "\n";
            // for(int i=0;i<32;i++) {
            //	std::cout << (int)decrypted[i] << "\n";
            // }

            decomp = calloc(size, 1);
            int n = sinflate(decomp, (int)size, &decrypted[16],
                             decrypted.size() - 16);

            if (!loader.LoadASCIIFromString(
                    &model, &warn, &err, reinterpret_cast<const char *>(decomp),
                    size, "/")) {
                throw std::runtime_error(warn + err);
            }
        } else {
            if (!loader.LoadASCIIFromFile(&model, &warn, &err, file.c_str())) {
                throw std::runtime_error(warn + err);
            }
        }

        for (const auto &mesh : model.meshes) {
            std::cout << "Primitives: " << mesh.primitives.size() << "\n";
            for (const auto &primitive : mesh.primitives) {
                if (primitive.indices < 0) {
                    continue;
                }

                const float *bufferPos = nullptr;
                const float *bufferNormals = nullptr;
                const float *bufferTangents = nullptr;
                const float *bufferTexCoords = nullptr;

                bool meshHasPos = false;
                bool meshHasNorm = false;
                bool meshHasTan = false;
                bool meshHasUV = false;

                int cntPos = 0;
                int cntNorm = 0;
                int cntTan = 0;
                int cntUV = 0;
                int cntTot = 0;

                auto pIt = primitive.attributes.find("POSITION");
                if (pIt != primitive.attributes.end()) {
                    const tinygltf::Accessor &posAccessor =
                        model.accessors[pIt->second];
                    const tinygltf::BufferView &posView =
                        model.bufferViews[posAccessor.bufferView];
                    bufferPos = reinterpret_cast<const float *>(
                        &(model.buffers[posView.buffer]
                              .data[posAccessor.byteOffset +
                                    posView.byteOffset]));
                    meshHasPos = true;
                    cntPos = posAccessor.count;
                    if (cntPos > cntTot) cntTot = cntPos;
                } else {
                    if (VD->Position.hasIt) {
                        std::cout << "Warning: vertex layout has position, but "
                                     "file hasn't\n";
                    }
                }

                auto nIt = primitive.attributes.find("NORMAL");
                if (nIt != primitive.attributes.end()) {
                    const tinygltf::Accessor &normAccessor =
                        model.accessors[nIt->second];
                    const tinygltf::BufferView &normView =
                        model.bufferViews[normAccessor.bufferView];
                    bufferNormals = reinterpret_cast<const float *>(
                        &(model.buffers[normView.buffer]
                              .data[normAccessor.byteOffset +
                                    normView.byteOffset]));
                    meshHasNorm = true;
                    cntNorm = normAccessor.count;
                    if (cntNorm > cntTot) cntTot = cntNorm;
                } else {
                    if (VD->Normal.hasIt) {
                        std::cout << "Warning: vertex layout has normal, but "
                                     "file hasn't\n";
                    }
                }

                auto tIt = primitive.attributes.find("TANGENT");
                if (tIt != primitive.attributes.end()) {
                    const tinygltf::Accessor &tanAccessor =
                        model.accessors[tIt->second];
                    const tinygltf::BufferView &tanView =
                        model.bufferViews[tanAccessor.bufferView];
                    bufferTangents = reinterpret_cast<const float *>(
                        &(model.buffers[tanView.buffer]
                              .data[tanAccessor.byteOffset +
                                    tanView.byteOffset]));
                    meshHasTan = true;
                    cntTan = tanAccessor.count;
                    if (cntTan > cntTot) cntTot = cntTan;
                } else {
                    if (VD->Tangent.hasIt) {
                        std::cout << "Warning: vertex layout has tangent, but "
                                     "file hasn't\n";
                    }
                }

                auto uIt = primitive.attributes.find("TEXCOORD_0");
                if (uIt != primitive.attributes.end()) {
                    const tinygltf::Accessor &uvAccessor =
                        model.accessors[uIt->second];
                    const tinygltf::BufferView &uvView =
                        model.bufferViews[uvAccessor.bufferView];
                    bufferTexCoords = reinterpret_cast<const float *>(&(
                        model.buffers[uvView.buffer]
                            .data[uvAccessor.byteOffset + uvView.byteOffset]));
                    meshHasUV = true;
                    cntUV = uvAccessor.count;
                    if (cntUV > cntTot) cntTot = cntUV;
                } else {
                    if (VD->UV.hasIt) {
                        std::cout << "Warning: vertex layout has UV, but file "
                                     "hasn't\n";
                    }
                }

                for (int i = 0; i < cntTot; i++) {
                    Vert vertex{};

                    if ((i < cntPos) && meshHasPos && VD->Position.hasIt) {
                        glm::vec3 pos = {bufferPos[3 * i + 0],
                                         bufferPos[3 * i + 1],
                                         bufferPos[3 * i + 2]};
                        glm::vec3 *o = (glm::vec3 *)((char *)(&vertex) +
                                                     VD->Position.offset);
                        *o = pos;
                    }

                    if ((i < cntNorm) && meshHasNorm && VD->Normal.hasIt) {
                        glm::vec3 normal = {bufferNormals[3 * i + 0],
                                            bufferNormals[3 * i + 1],
                                            bufferNormals[3 * i + 2]};
                        glm::vec3 *o = (glm::vec3 *)((char *)(&vertex) +
                                                     VD->Normal.offset);
                        *o = normal;
                    }

                    if ((i < cntTan) && meshHasTan && VD->Tangent.hasIt) {
                        glm::vec4 tangent = {bufferTangents[4 * i + 0],
                                             bufferTangents[4 * i + 1],
                                             bufferTangents[4 * i + 2],
                                             bufferTangents[4 * i + 3]};
                        glm::vec4 *o = (glm::vec4 *)((char *)(&vertex) +
                                                     VD->Tangent.offset);
                        *o = tangent;
                    }

                    if ((i < cntUV) && meshHasUV && VD->UV.hasIt) {
                        glm::vec2 texCoord = {bufferTexCoords[2 * i + 0],
                                              bufferTexCoords[2 * i + 1]};
                        glm::vec2 *o =
                            (glm::vec2 *)((char *)(&vertex) + VD->UV.offset);
                        *o = texCoord;
                    }

                    vertices.push_back(vertex);
                }

                const tinygltf::Accessor &accessor =
                    model.accessors[primitive.indices];
                const tinygltf::BufferView &bufferView =
                    model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer =
                    model.buffers[bufferView.buffer];

                switch (accessor.componentType) {
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
                        const uint16_t *bufferIndex =
                            reinterpret_cast<const uint16_t *>(
                                &(buffer.data[accessor.byteOffset +
                                              bufferView.byteOffset]));
                        for (int i = 0; i < accessor.count; i++) {
                            indices.push_back(bufferIndex[i]);
                        }
                    } break;
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
                        const uint32_t *bufferIndex =
                            reinterpret_cast<const uint32_t *>(
                                &(buffer.data[accessor.byteOffset +
                                              bufferView.byteOffset]));
                        for (int i = 0; i < accessor.count; i++) {
                            indices.push_back(bufferIndex[i]);
                        }
                    } break;
                    default:
                        std::cerr << "Index component type "
                                  << accessor.componentType << " not supported!"
                                  << std::endl;
                        throw std::runtime_error(
                            "Error loading GLTF component");
                }
            }
        }

        std::cout << (encoded ? "[MGCG]" : "[GLTF]")
                  << " Vertices: " << vertices.size()
                  << "\nIndices: " << indices.size() << "\n";
}

template <class Vert>
void Model<Vert>::createVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        BP->createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         vertexBuffer, vertexBufferMemory);

        void *data;
        vkMapMemory(BP->device, vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(BP->device, vertexBufferMemory);
}

template <class Vert>
void Model<Vert>::createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        BP->createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         indexBuffer, indexBufferMemory);

        void *data;
        vkMapMemory(BP->device, indexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(BP->device, indexBufferMemory);
}

template <class Vert>
void Model<Vert>::initMesh(BaseProject *bp, VertexDescriptor *vd) {
        BP = bp;
        VD = vd;
        std::cout << "[Manual] Vertices: " << vertices.size()
                  << "\nIndices: " << indices.size() << "\n";
        createVertexBuffer();
        createIndexBuffer();
}

template <class Vert>
void Model<Vert>::init(BaseProject *bp, VertexDescriptor *vd, std::string file,
                       ModelType MT) {
        BP = bp;
        VD = vd;
        if (MT == OBJ) {
            loadModelOBJ(file);
        } else if (MT == GLTF) {
            loadModelGLTF(file, false);
        } else if (MT == MGCG) {
            loadModelGLTF(file, true);
        }

        createVertexBuffer();
        createIndexBuffer();
}

template <class Vert>
void Model<Vert>::cleanup() {
        vkDestroyBuffer(BP->device, indexBuffer, nullptr);
        vkFreeMemory(BP->device, indexBufferMemory, nullptr);
        vkDestroyBuffer(BP->device, vertexBuffer, nullptr);
        vkFreeMemory(BP->device, vertexBufferMemory, nullptr);
}

template <class Vert>
void Model<Vert>::bind(VkCommandBuffer commandBuffer) {
        VkBuffer vertexBuffers[] = {vertexBuffer};
        // property .vertexBuffer of models, contains the VkBuffer handle to its
        // vertex buffer
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        // property .indexBuffer of models, contains the VkBuffer handle to its
        // index buffer
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0,
                             VK_INDEX_TYPE_UINT32);
}

#endif // MODEL_IMPLEMENTATION_