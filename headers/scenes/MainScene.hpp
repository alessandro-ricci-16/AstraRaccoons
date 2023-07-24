#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/scenes/Scene.hpp"

class MainScene: public Scene {
    private:
        float lastSpawnTime;
        float spawnDeltaTime;
        GameObject* player;
        const unsigned int initialAsteroids = 10, maxVisibleAsteroids = 150;
        int visibleAsteroids = 0;
        bool restartFlag = false;

    public:
        MainScene(float* ar) : Scene(ar) {}
        void Instantiate() override;
        void Update() override;
        void Destroy() override;
        void Cleanup() override;
        void WillDisappear() override;

        void addObject(GameObject* object) override;
        void removeObject(GameObject* object) override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_
