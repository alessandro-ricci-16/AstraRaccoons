#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_TESTSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_TESTSCENE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/scenes/Scene.hpp"

class TestScene: public Scene {
    public:
        void Instantiate();
        void Update();
        void Destroy();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_TESTSCENE_HPP_