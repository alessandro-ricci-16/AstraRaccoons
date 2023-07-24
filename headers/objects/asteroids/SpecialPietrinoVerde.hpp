#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVERDEOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVERDEOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoVerdeUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoVerde : public AbstractAsteroidObject {
    private:
        SpecialPietrinoVerdeUniforms additionalUniforms;
        void die() override;
        void addScore(int points) override;
    public:
        SpecialPietrinoVerde(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVERDEOBJECT_HPP_