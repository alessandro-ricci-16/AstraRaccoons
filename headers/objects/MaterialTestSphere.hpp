#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTSPHERE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTSPHERE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"

struct TestSphereVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class TestSphereObject : public MeshObject<TestSphereVertex>, public ICollidable {
   public:
    bool moves;
    void Instantiate();
    void Start();
    void Update();
    void OnCollisionWith(GameObject* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_MATERIALTESTSPHERE_HPP_