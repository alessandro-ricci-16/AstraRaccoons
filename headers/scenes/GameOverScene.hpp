#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_GAMEOVERSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_GAMEOVERSCENE_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/scenes/Scene.hpp>

class GameOverScene: public Scene {
    private:
        Transform* lookAtTarget;
        bool sceneSwitchAllowed = false;

    public:
        GameOverScene(float* ar) : Scene(ar) {}
        void Instantiate();
        void Update();
        void Destroy();
        void Cleanup();
        void WillDisappear();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_GAMEOVERSCENE_HPP_