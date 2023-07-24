#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVIOLAOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVIOLAOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoViolaUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoViola : public AbstractAsteroidObject {
    private:
        SpecialPietrinoViolaUniforms additionalUniforms;
        void die() override;
    public:
        SpecialPietrinoViola(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOVIOLAOBJECT_HPP_
