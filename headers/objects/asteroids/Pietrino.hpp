#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRINOOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRINOOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

class Pietrino : public AbstractAsteroidObject {

public:
    Pietrino(GameObject* player) : AbstractAsteroidObject(player) {}

    void Instantiate() override;
    std::string getModelName() override;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRINOOBJECT_HPP_
