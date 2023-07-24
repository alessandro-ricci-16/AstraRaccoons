#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/scenes/Scene.hpp"
#include "../objects/Score.hpp"

class MainScene: public Scene {
    private:
        float spawnTimer;
        float spawnDeltaTime;
        const float initialSpawnDeltaTime = 3.0f;
        const float loweringPerAsteroid = 0.1f;
        const float minSpawnRate = 0.1f;
        GameObject* player = nullptr;
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
