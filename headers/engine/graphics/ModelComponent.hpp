#ifndef MODELCOMPONENT_INCLUDE
#define MODELCOMPONENT_INCLUDE

#include "../base/includes.hpp"
#include "../base/BaseProject.hpp"
#include "../base/Model.hpp"
#include "../base/Texture.hpp"
#include "ObjectVertexDescriptor.hpp"
#include "GraphicsPipeline.hpp"

template <class Vert>
class ModelComponent {
   private:
        std::string modelName;
        std::string vertexStage, fragmentStage;
        std::vector<std::string> textureNames;

   protected:
        uint32_t id;
        Model<Vert> model;
        ObjectVertexDescriptor vertexDescriptor;
        GraphicsPipeline pipeline;
        std::vector<Texture> textures;

   public:
        Uniforms uniforms;

        ModelComponent(std::string name, ObjectVertexDescriptor vertexDescriptor);

        void addTexture(std::string name);
        void setShader(std::string vertexStage, std::string fragmentStage);
        void compile(BaseProject* proj, bool withGUBO);

        void cleanup();
        void destroy();
};

#endif

#ifdef MODELCOMPONENT_IMPLEMENTATION_
#undef MODELCOMPONENT_IMPLEMENTATION_

#include <unordered_map>

template <class Vert>
ModelComponent<Vert>::ModelComponent(std::string name,
                                     ObjectVertexDescriptor vertexDescriptor) {
    modelName = name;
    vertexStage = "";
    fragmentStage = "";
    textureNames = {};
    textures = {};
    // Generate the ID from the hash of the model name (used to recycle models)
    std::hash<std::string> hasher;
    id = hasher(name);
    this->vertexDescriptor = vertexDescriptor;
}

template <class Vert>
void ModelComponent<Vert>::addTexture(std::string name) {
    textureNames.push_back(name);
}

template <class Vert>
void ModelComponent<Vert>::setShader(std::string vertexStage,
                                     std::string fragmentStage) {
    this->vertexStage = vertexStage;
    this->fragmentStage = fragmentStage;
    pipeline = GraphicsPipeline(vertexStage, fragmentStage);
}

template <class Vert>
void ModelComponent<Vert>::compile(BaseProject* proj, bool withGUBO) {
    // Compile textures
    for (int i = 0; i < textureNames.size(); i++) {
        Texture tex;
        tex.init(proj, textureNames.at(i).c_str());
        textures.push_back(tex);
    }
    // Compile the vertex descriptor & model
    VertexDescriptor vd = vertexDescriptor.compile(proj);
    //Derive the model type from the file extension
    std::filesystem::path modelPath = modelName;
    std::string modelExtension = modelPath.extension();
    ModelType modelType;
    if (modelExtension == ".obj") {
        modelType = OBJ;
    } else if (modelExtension == ".gltf") {
        modelType = GLTF;
    } else {
        modelType = MGCG;
    }
    model.init(proj, &vd, modelName, modelType);
    //Compile the pipeline
    //Add all necessary sets & descriptor bindings
    if (withGUBO) {
        //Bind Global uniforms
        pipeline.addSet();
        pipeline.addUniformBindingToLastSet(sizeof(GlobalUniforms), VK_SHADER_STAGE_ALL_GRAPHICS);
    }
    //Bind model uniforms
    pipeline.addSet();
    pipeline.addUniformBindingToLastSet(sizeof(Uniforms), VK_SHADER_STAGE_ALL_GRAPHICS);
    //Bind all textures in order
    for (int i = 0; i < textures.size(); i++) {
        Texture tex = textures.at(i);
        pipeline.addTextureBindingToLastSet(&tex, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    //Compile the pipeline
    pipeline.compile(proj, &vd);
}

template <class Vert>
void ModelComponent<Vert>::cleanup() {
    for (int i = 0; i < textures.size(); i++) {
        Texture tex = textures.at(i);
        tex.cleanup();
    }
    pipeline.cleanup();
    model.cleanup();
}

template <class Vert>
void ModelComponent<Vert>::destroy() {
    for (int i = 0; i < textures.size(); i++) {
        Texture tex = textures.at(i);
        tex.cleanup();
    }
    pipeline.destroy();
    model.cleanup();
}

#endif  // MODELCOMPONENT_IMPLEMENTATION_