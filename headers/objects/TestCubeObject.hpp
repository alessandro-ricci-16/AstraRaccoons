#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/objects/MeshObject.hpp"

struct TestCubeVertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

class TestCubeObject: public MeshObject<TestCubeVertex> {
    public:
        void Instantiate();
        void Start();
        void Update();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_TESTCUBEOBJECT_HPP_