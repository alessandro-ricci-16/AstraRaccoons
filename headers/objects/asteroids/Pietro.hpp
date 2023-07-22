#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETROOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETROOBJECT_HPP_

#include "AbstractAsteroidObject.hpp"

class Pietro : public AbstractAsteroidObject {

public:
    Pietro(Transform* playerTransform) : AbstractAsteroidObject(playerTransform) {}

    void Instantiate() override;
    std::string getModelName();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_PIETROOBJECT_HPP_