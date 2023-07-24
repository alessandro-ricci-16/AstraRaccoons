#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_

#include "../engine/base/includes.hpp"
#include "../engine/scenes/Scene.hpp"
#include "../objects/Score.hpp"

class MainScene: public Scene {
    private:
        Score* score = nullptr;
        float lastSpawnTime;
        float spawnDeltaTime;
        GameObject* player = nullptr;
        const unsigned int initialAsteroids = 10, maxVisibleAsteroids = 150;
        int visibleAsteroids = 0;
        bool restartFlag = false;

    public:
        MainScene(float* ar) : Scene(ar) {}
        void addScore(int points);
        void Instantiate();
        void Update();
        void Destroy();
        void Cleanup();
        void WillDisappear();

        void addObject(GameObject* object) override;
        void removeObject(GameObject* object) override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_SCENES_MAINSCENE_HPP_