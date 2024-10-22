#ifndef MODELCOMPONENT_INCLUDE
#define MODELCOMPONENT_INCLUDE

#include "../base/includes.hpp"
#include "../base/BaseProject.hpp"
#include "../base/Model.hpp"
#include "../base/Texture.hpp"
#include "ObjectVertexDescriptor.hpp"
#include "GraphicsPipeline.hpp"
#include <headers/engine/base/TexturePool.hpp>

template <class Vert>
class ModelComponent {
   private:
        struct UniformEntry {
            void* uniformPtr;
            int size;
            VkShaderStageFlagBits stage;
        };

        struct TextureEntry {
            const char** filenames;
            int size;
        };

        std::string modelName;          // For loaded models
        std::vector<Vert> verts;        // For procedural meshes
        std::vector<uint32_t> indices;  // For procedural meshes
        bool isProcedural;
        std::string vertexStage, fragmentStage;
        std::vector<std::vector<std::string>> textureNames;
        std::vector<UniformEntry> additionalUniforms;
        bool isCompiled;

   protected:
        uint32_t id;
        Model<Vert> model;
        ObjectVertexDescriptor* vertexDescriptor;
        GraphicsPipeline* pipeline;
        std::vector<Texture*> textures;

   public:
        Uniforms uniforms;
        bool isTransparent, backfaceCullingOn;

        ModelComponent() = default;
        ModelComponent(std::string name, ObjectVertexDescriptor* vertexDescriptor);
        ModelComponent(std::vector<Vert> verts, std::vector<uint32_t> indices, ObjectVertexDescriptor* vertexDescriptor);

        void addTexture(std::string name);
        void addCubicTexture(std::vector<std::string> files);
        void addUniformData(void* uniformPtr, int size, VkShaderStageFlagBits stage);
        void setShader(std::string vertexStage, std::string fragmentStage);
        void compile(BaseProject* proj, GlobalUniforms* guboPtr);
        void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline);
        void Commit(int currentImage);

        int textureCount();
        int uniformCount();

        void cleanup();
        void destroy();
};

#endif

#ifdef MODELCOMPONENT_IMPLEMENTATION_
#undef MODELCOMPONENT_IMPLEMENTATION_

#define MODEL_IMPLEMENTATION_
#include "../base/Model.hpp"

#include <unordered_map>
#include "../base/Random.hpp"

template <class Vert>
ModelComponent<Vert>::ModelComponent(std::string name, ObjectVertexDescriptor* vertexDescriptor) {
    modelName = name;
    vertexStage = "";
    fragmentStage = "";
    textureNames = {};
    textures = {};
    // Generate the ID from the hash of the model name (used to recycle models)
    std::hash<std::string> hasher;
    id = (uint32_t)hasher(name);
    this->vertexDescriptor = vertexDescriptor;
    isCompiled = false;
    isProcedural = false;
    isTransparent = false;
    backfaceCullingOn = true;
}

template <class Vert>
ModelComponent<Vert>::ModelComponent(std::vector<Vert> verts, std::vector<uint32_t> indices, ObjectVertexDescriptor* vertexDescriptor) {
    this->verts = {};
    this->indices = {};
    copy(verts.begin(), verts.end(), back_inserter(this->verts));
    copy(indices.begin(), indices.end(), back_inserter(this->indices));
    modelName = "";
    vertexStage = "";
    fragmentStage = "";
    textureNames = {};
    textures = {};
    // Generate the ID from the hash of the model name (used to recycle models)
    id = (uint32_t)Random::randomInt(0, UINT32_MAX);
    this->vertexDescriptor = vertexDescriptor;
    isCompiled = false;
    isProcedural = true;
    isTransparent = false;
    backfaceCullingOn = true;
}

template <class Vert>
void ModelComponent<Vert>::addTexture(std::string name) {
    textureNames.push_back({ name });
}

template <class Vert>
void ModelComponent<Vert>::addCubicTexture(std::vector<std::string> files) {
    textureNames.push_back(files);
}

template <class Vert>
void ModelComponent<Vert>::addUniformData(void* uniformPtr, int size, VkShaderStageFlagBits stage) {
    additionalUniforms.push_back({ uniformPtr, size, stage });
}

template <class Vert>
int ModelComponent<Vert>::textureCount() {
    return (int)textureNames.size();
}

template <class Vert>
int ModelComponent<Vert>::uniformCount() {
    return 1 + (int)additionalUniforms.size();
}

template <class Vert>
void ModelComponent<Vert>::setShader(std::string vertexStage,
                                     std::string fragmentStage) {
    this->vertexStage = vertexStage;
    this->fragmentStage = fragmentStage;
    pipeline = new GraphicsPipeline(vertexStage, fragmentStage);
}

template <class Vert>
void ModelComponent<Vert>::compile(BaseProject* proj, GlobalUniforms* guboPtr) {
    // Compile the vertex descriptor & model
    VertexDescriptor* vd = vertexDescriptor->compile(proj);
    //Compile assets only if they have been cleaned up
    if (!isCompiled) {
        // Compile textures
        for (int i = 0; i < textureNames.size(); i++) {
            Texture* tex = TexturePool::getTexture(textureNames[i]);
            if (tex == nullptr) {
                tex = new Texture();
                if (textureNames[i].size() == 1) {
                    tex->init(proj, textureNames.at(i).at(0).c_str());
                } else {
                    const char* filenames[6];
                    for (int j = 0; j < 6; j++) {
                        filenames[j] = textureNames.at(i).at(j).c_str();
                    }
                    tex->initCubic(proj, filenames);
                }
                TexturePool::addTextureIfNotPresent(textureNames.at(i), tex);
            } else {
                #ifdef DEBUG
                    std::cout << "Retrieved texture ";
                    for (std::string str : textureNames[i]) {
                        std::cout << str << " - ";
                    }
                    std::cout << "\n";
                #endif // DEBUG
                TexturePool::registerTextureUsage(textureNames[i]);
            }
            textures.push_back(tex);
        }
        if (!isProcedural) {
            // Derive the model type from the file extension
            std::filesystem::path modelPath = modelName;
            std::string modelExtension = modelPath.extension().string();
            ModelType modelType;
            if (modelExtension == ".obj") {
                modelType = OBJ;
            } else if (modelExtension == ".gltf") {
                modelType = GLTF;
            } else {
                modelType = MGCG;
            }
            model.init(proj, vd, modelName, modelType);
        } else {
            model.vertices = verts;
            model.indices = indices;
            model.initMesh(proj, vd);
        }
        
        //Compile the pipeline - this needs to be done even if all assets were compiled, since the pipeline may have been cleaned up
        //Add all necessary sets & descriptor bindings
        if (guboPtr != nullptr) {
            //Bind Global uniforms
            pipeline->addSet(true);
            pipeline->addUniformBindingToLastSet(guboPtr, sizeof(GlobalUniforms), VK_SHADER_STAGE_ALL_GRAPHICS);
        }
        //Bind model uniforms
        pipeline->addSet(false);
        pipeline->addUniformBindingToLastSet(&uniforms, sizeof(Uniforms), VK_SHADER_STAGE_ALL_GRAPHICS);
        //Bind all textures in order
        for (int i = 0; i < textures.size(); i++) {
            Texture* tex = textures.at(i);
            pipeline->addTextureBindingToLastSet(tex, VK_SHADER_STAGE_FRAGMENT_BIT);
        }
        //Bind each additional uniform in a separate set
        for (int i = 0; i < additionalUniforms.size(); i++) {
            UniformEntry entry = additionalUniforms.at(i);
            pipeline->addSet(false);
            pipeline->addUniformBindingToLastSet(entry.uniformPtr, entry.size, entry.stage);
        }
    }

    pipeline->isTransparent = isTransparent;
    pipeline->backfaceCullingOn = backfaceCullingOn;
    
    //Compile the pipeline
    pipeline->compile(proj, vd);

    isCompiled = true;
}

template <class Vert>
void ModelComponent<Vert>::cleanup() {
    pipeline->cleanup();
}

template <class Vert>
void ModelComponent<Vert>::destroy() {
    for (int i = 0; i < textureNames.size(); i++) {
        TexturePool::removeTexture(textureNames[i]);
    }
    textureNames.clear();
    textures.clear();
    pipeline->destroy();
    model.cleanup();
    isCompiled = false;
    delete vertexDescriptor;
}

template <class Vert>
void ModelComponent<Vert>::Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) {
    //Uniforms are automatically updated every frame since the pipeline maps them using the direct pointer
    //Bind the pipeline and uniforms
    pipeline->bind(commandBuffer, currentImage, activePipeline);
    //Bind the model
    model.bind(commandBuffer);
    //Draw!
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(model.indices.size()), 1, 0, 0, 0);   
}

template <class Vert>
void ModelComponent<Vert>::Commit(int currentImage) {
    //Update uniforms part of the pipeline
    pipeline->commitUniforms(currentImage);
}

#endif  // MODELCOMPONENT_IMPLEMENTATION_
