#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#include "../base/includes.hpp"
#include "../graphics/ModelComponent.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

template <class Vert>
class MeshObject: public GameObject {
    public:
        ModelComponent<Vert> model;
        virtual ~MeshObject();

        void setModel(std::string name, ObjectVertexDescriptor* vertexDescriptor);
        void compile(BaseProject* proj, GlobalUniforms* guboPtr) override;

        virtual void Instantiate() = 0;
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Destroy();
        virtual void Cleanup();
        void CommitUpdates(int currentimage, glm::mat4 cameraMatrix) override;

        void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix) override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#ifdef MESHOBJECT_IMPLEMENTATION
#undef MESHOBJECT_IMPLEMENTATION

#define MODELCOMPONENT_IMPLEMENTATION_
#include "../graphics/ModelComponent.hpp"

template <class Vert>
void MeshObject<Vert>::compile(BaseProject* proj, GlobalUniforms* guboPtr) {
    model.compile(proj, acceptsGUBOs ? guboPtr : nullptr);
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->compile(proj, guboPtr);
    }
}

template <class Vert>
void MeshObject<Vert>::setModel(std::string name, ObjectVertexDescriptor* vertexDescriptor) {
    model = ModelComponent<Vert>(name, vertexDescriptor);
}

template <class Vert>
void MeshObject<Vert>::Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix) {
    //Draw the model
    model.Draw(commandBuffer, currentImage, activePipeline);
    //Recursively draw all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Draw(commandBuffer, currentImage, activePipeline, cameraMatrix);
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
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->Destroy();
    }
}

template <class Vert>
void MeshObject<Vert>::Cleanup() {
    model.cleanup();
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