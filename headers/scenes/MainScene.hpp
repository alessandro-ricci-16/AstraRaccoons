#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/scenes/Scene.hpp"

class MainScene: public Scene {
    private:
        float lastSpawnTime;
        float spawnDeltaTime;
        GameObject* player;

    public:
        MainScene(float* ar) : Scene(ar) {}
        float time;
        void Instantiate();
        void Update();
        void Destroy();
        void Cleanup();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_