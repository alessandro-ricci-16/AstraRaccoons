#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSAOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSAOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoRosaUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoRosa : public AbstractAsteroidObject {
    private:
        SpecialPietrinoRosaUniforms additionalUniforms;
        void die() override;
        void addScore(int points) override;
    public:
        SpecialPietrinoRosa(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSAOBJECT_HPP_