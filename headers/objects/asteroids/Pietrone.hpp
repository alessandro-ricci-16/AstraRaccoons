#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRONEOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRONEOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

class Pietrone : public AbstractAsteroidObject {
   public:
    Pietrone(GameObject* player) : AbstractAsteroidObject(player) {}

    void Instantiate() override;
    std::string getModelName() override;
};

#endif  // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETRONEOBJECT_HPP_
