#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTPLANE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTPLANE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"

struct TestPlaneVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class TestPlaneObject : public MeshObject<TestPlaneVertex>, public ICollidable {
   public:
    bool moves;
    void Instantiate();
    void Update();
    void OnCollisionWith(Collider* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTPLANE_HPP_