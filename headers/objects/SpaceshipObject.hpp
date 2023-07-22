#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/objects/MeshObject.hpp>
#include <headers/engine/objects/Collider.hpp>

enum Effect {
    EFFECT_DAMAGE,
    EFFECT_TIME,
    EFFECT_FIRERATE,
    EFFECT_SIZEDOWN
};

struct SpaceshipVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 norm;
};

struct SpaceshipUniforms {
    alignas(16) glm::vec4 flashingColor;
    alignas(16) glm::vec4 emissionColor;
};

class SpaceshipObject : public MeshObject<SpaceshipVertex>, public ICollidable {
    private:
        void scaleColliders(float scale);
        SpaceshipUniforms additionalUniforms;

        // special effects
        int effects = 0; // mask
        const float effectDuration = 10.0; // seconds
        std::vector<float> effectTimers;

        const float effectDamageMultiplier = 2.f;
        const float effectTimeScale = 0.2f;
        float initialScale;
        const float effectScaleDownFactor = 3.0f;

        glm::vec3 vel;
        glm::vec3 angVel;
        // maxVel = a/d
        // acc = a - d*v
        const float acc = 20.0f;
        const float dec = 1.0f;
        const float angAcc = 250.0f;
        const float angDec = 2.0f;

        float emissionMultiplier = 0.0f;

        bool fire = false;
        bool reloading = true;
        bool firingAllowed = false;
        float timer = 0.0f;

        const float fireRate = 0.1f;
        const float shotSpeed = 150.0f;
        const float shotRange = 150.0f;
        const float shotDamage = 20.0f;
        const float shotThickness = 0.6f;

        int lives = 3;
        const int maxLives = 3;

        float disabledKeysTimer = 0.0f;
        const float disabledKeysDefaultTimer = 2.2f;
        const glm::vec4 baseFlashingColor = glm::vec4(0.878f, 0.224f, 0.024f, 0.35f);
        bool flashingEnabled = false;

        const glm::vec3 shotColor = glm::vec3(1, 0, 0);
        glm::vec3 shotOffset = glm::vec3(4.4f, -0.3f, -2.25f);

    public:
        glm::vec3 getVelocity();
        bool hadRecentCollision();

        void Instantiate();
        void Update();
        void OnCollisionWith(Collider* other);
        bool hasEffect(Effect e);
        void setEffect(Effect e);
        void unsetEffect(Effect e);
        void resetLives();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_