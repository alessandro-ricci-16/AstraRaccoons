#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_

#include <headers/engine/base/includes.hpp>
#include <headers/engine/objects/MeshObject.hpp>
#include <headers/engine/objects/Collider.hpp>

enum Effect {
    EFFECT_DAMAGE,
    EFFECT_TIME,
    EFFECT_FIRERATE,
    EFFECT_SIZEDOWN,
    EFFECT_SPEEDUP,
    TotEffects
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
        void die();
        void scaleColliders(float scale);
        void updateEffectTimers(float deltaT);
        SpaceshipUniforms additionalUniforms;

        // special effects
        int effects = 0; // mask
        const float effectDuration = 10.0; // seconds
        std::vector<float> effectTimers;

        const float effectDamageMultiplier = 2.f;
        const float effectTimeScale = 0.2f;
        float initialScale;
        const float effectScaleDownFactor = 3.0f;
        const float effectSpeedUpFactor = 3.0f;
        const glm::vec3 normalEmissionColor = glm::vec3(1.f, 0.723f, 0.022f);
        const glm::vec3 speedUpEmissionColor = glm::vec3(1.f, 0.05f, 0.05f);

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

        bool flashingEffect = false;
        float disabledKeysTimer = 0.0f;
        const float disabledKeysDefaultTimer = 2.5f;
        const glm::vec4 damageFlashingColor = glm::vec4(0.878f, 0.224f, 0.024f, 0.35f);
        float hpFlashingTimer = 0.0f;
        float hpFlashingDefaultTimer = 2.5f;
        const glm::vec4 hpUpFlashingColor = glm::vec4(0.2f, 1.0f, 0.0f, 0.35f);

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
        void setLives(int delta);
        void unsetEffect(Effect e);
        void resetLives();
        int getLives();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SPACESHIP_HPP_