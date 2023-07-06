#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#include "../base/includes.hpp"
#include "../graphics/ModelComponent.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

template <class Vert>
class MeshObject: public GameObject {
    private:
        ModelComponent<Vert> model;

    public:
        void setModel(std::string name, ObjectVertexDescriptor vertexDescriptor);

        virtual void Instantiate();
        virtual void Start();
        virtual void Update();
        virtual void Destroy();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MESHOBJECT_HPP_

#ifdef MESHOBJECT_IMPLEMENTATION
#undef MESHOBJECT_IMPLEMENTATION

template <class Vert>
void MeshObject<Vert>::setModel(std::string name, ObjectVertexDescriptor vertexDescriptor) {
    model = ModelComponent<Vert>(name, vertexDescriptor)
}

#endif  // MESHOBJECT_IMPLEMENTATION