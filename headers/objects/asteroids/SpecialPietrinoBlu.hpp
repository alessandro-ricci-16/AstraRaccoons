#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOBLUOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOBLUOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

struct SpecialPietrinoBluUniforms {
    alignas(16) glm::vec4 color;
};

class SpecialPietrinoBlu : public AbstractAsteroidObject {
    private:
        SpecialPietrinoBluUniforms additionalUniforms;
        void die() override;
        void addScore(int points) override;
    public:
        SpecialPietrinoBlu(GameObject* player) : AbstractAsteroidObject(player) {}

        void Instantiate() override;
        std::string getModelName() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_SPECIALPIETRINOBLUOBJECT_HPP_
