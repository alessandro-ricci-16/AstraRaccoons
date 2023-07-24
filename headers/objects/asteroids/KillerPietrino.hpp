#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_KILLERPIETRINOOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_KILLERPIETRINOOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct KillerPietrinoUniforms {
    alignas(4) float time;
};

class KillerPietrino : public AbstractAsteroidObject {
    private:
        KillerPietrinoUniforms additionalUniforms;
        void die() override;
        void receiveDamage(float damage) override;
    public:
        KillerPietrino(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        void Update() override;
        std::string getModelName() override;
};

#endif  // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_KILLERPIETRINOOBJECT_HPP_
