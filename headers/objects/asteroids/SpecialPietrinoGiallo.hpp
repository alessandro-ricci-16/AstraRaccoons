#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOGIALLOOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOGIALLOOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoGialloUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoGiallo : public AbstractAsteroidObject {
    private:
        SpecialPietrinoGialloUniforms additionalUniforms;
        void die() override;
        void addScore(int points) override;
    public:
        SpecialPietrinoGiallo(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOGIALLOOBJECT_HPP_
