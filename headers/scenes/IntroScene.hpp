#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_INTROSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_INTROSCENE_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/scenes/Scene.hpp>
#include <headers/objects/TextMaker.hpp>

class IntroScene: public Scene {
    private:
        Transform* lookAtTarget;
        Game* game;
        const char* text;

    public:
        IntroScene(float* ar) : Scene(ar) {}
        void Instantiate();
        void Update();
        void Destroy();
        void Cleanup();
        void WillDisappear();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_INTROSCENE_HPP_