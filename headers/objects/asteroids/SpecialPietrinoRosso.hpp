#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSSOOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSSOOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoRossoUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoRosso : public AbstractAsteroidObject {
    private:
        SpecialPietrinoRossoUniforms additionalUniforms;
        void die() override;
        void addScore(int points) override;
    public:
        SpecialPietrinoRosso(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOROSSOOBJECT_HPP_
