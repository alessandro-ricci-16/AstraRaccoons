#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"
#include <headers/engine/objects/Collider.hpp>

struct TestCubeVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

class TestCubeObject: public MeshObject<TestCubeVertex>, public ICollidable {
    public:
        bool moves;
        void Instantiate();
        void Update();
        void OnCollisionWith(Collider* other);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_