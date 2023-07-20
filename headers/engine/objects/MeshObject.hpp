#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#include "../base/includes.hpp"
#include "../graphics/ModelComponent.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

template <class Vert>
class MeshObject: public GameObject {
    private:
        bool isCompiled = false;
    public:
        ModelComponent<Vert> model;
        virtual ~MeshObject();

        void setModel(std::string name, ObjectVertexDescriptor* vertexDescriptor);
        void setModel(std::vector<Vert> vertices, std::vector<uint32_t> indices, ObjectVertexDescriptor* vertexDescriptor);
        void compile(BaseProject* proj, GlobalUniforms* guboPtr) override;

        virtual void Instantiate() = 0;
        virtual void Update() = 0;
        virtual void Destroy();
        virtual void Cleanup();
        void CommitUpdates(int currentimage, glm::mat4 cameraMatrix) override;

        int textureCount() override;
        int uniformsCount() override;

        void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#ifdef MESHOBJECT_IMPLEMENTATION
#undef MESHOBJECT_IMPLEMENTATION

#define MODELCOMPONENT_IMPLEMENTATION_
#include "../graphics/ModelComponent.hpp"

template <class Vert>
void MeshObject<Vert>::compile(BaseProject* proj, GlobalUniforms* guboPtr) {
    if (!isCompiled) {
        model.compile(proj, acceptsGUBOs ? guboPtr : nullptr);
        isCompiled = true;
    }
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->compile(proj, guboPtr);
    }
}

template <class Vert>
void MeshObject<Vert>::setModel(std::string name, ObjectVertexDescriptor* vertexDescriptor) {
    model = ModelComponent<Vert>(name, vertexDescriptor);
}

template <class Vert>
void MeshObject<Vert>::setModel(std::vector<Vert> vertices, std::vector<uint32_t> indices, ObjectVertexDescriptor* vertexDescriptor) {
    model = ModelComponent<Vert>(vertices, indices, vertexDescriptor);
}

template <class Vert>
int MeshObject<Vert>::textureCount() {
    int myTexCount = model.textureCount();
    for (int i = 0; i < children.size(); i++) {
        myTexCount += children[i]->textureCount();
    }
    return myTexCount;
}

template <class Vert>
int MeshObject<Vert>::uniformsCount() {
    int myUniformCount = model.uniformCount() + (acceptsGUBOs ? 1 : 0);
    for (int i = 0; i < children.size(); i++) {
        myUniformCount += children[i]->uniformsCount();
    }
    return myUniformCount;
}

template <class Vert>
void MeshObject<Vert>::Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline) {
    //Draw the model
    model.Draw(commandBuffer, currentImage, activePipeline);
    //Recursively draw all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Draw(commandBuffer, currentImage, activePipeline);
    }
    
}

template <class Vert>
void MeshObject<Vert>::CommitUpdates(int currentImage, glm::mat4 cameraMatrix) {
    //Update model uniforms with the object transform
    model.uniforms.modelMat = transform.getMatrix();
    model.uniforms.nMat = glm::inverse(glm::transpose(model.uniforms.modelMat));
    model.uniforms.cameraMat = cameraMatrix;
    model.uniforms.mvpMat = cameraMatrix * model.uniforms.modelMat;
    //Commit updates
    model.Commit(currentImage);
    //Recursively update all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->CommitUpdates(currentImage, cameraMatrix);
    }
}

template <class Vert>
void MeshObject<Vert>::Destroy() {
    model.destroy();
    isCompiled = false;
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Destroy();
    }
}

template <class Vert>
void MeshObject<Vert>::Cleanup() {
    model.cleanup();
    isCompiled = false;
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Cleanup();
    }
}

template <class Vert>
MeshObject<Vert>::~MeshObject() {
    for (int i = 0; i < children.size(); i++) {
        delete children.at(i);
    }
    children.clear();
}

#endif  // MESHOBJECT_IMPLEMENTATION