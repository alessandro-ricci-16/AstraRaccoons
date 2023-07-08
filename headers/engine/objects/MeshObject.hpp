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

        void setModel(std::string name, ObjectVertexDescriptor* vertexDescriptor);
        virtual void compile(BaseProject* proj, GlobalUniforms* guboPtr);

        virtual void Instantiate() = 0;
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
        virtual void CommitUpdates(int currentimage, glm::mat4 cameraMatrix);

        virtual void Draw(VkCommandBuffer commandBuffer, int currentImage, GraphicsPipeline* activePipeline, glm::mat4 cameraMatrix);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#ifdef MESHOBJECT_IMPLEMENTATION
#undef MESHOBJECT_IMPLEMENTATION

#define MODELCOMPONENT_IMPLEMENTATION_
#include "../graphics/ModelComponent.hpp"

template <class Vert>
void MeshObject<Vert>::compile(BaseProject* proj, GlobalUniforms* guboPtr) {
    model.compile(proj, guboPtr);
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
    model.uniforms.mvpMat = cameraMatrix * transform.getMatrix();
    //Commit updates
    model.Commit(currentImage);
    //Recursively update all children
    for (int i = 0; i < children.size(); i++) {
        children.at(i)->CommitUpdates(currentImage, cameraMatrix);
    }
}

#endif  // MESHOBJECT_IMPLEMENTATION