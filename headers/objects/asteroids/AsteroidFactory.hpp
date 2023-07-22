#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#include "../../engine/scenes/Scene.hpp"
#include "../../objects/asteroids/AbstractAsteroidObject.hpp"
#include "../../engine/base/includes.hpp"

using AsteroidConstructor = std::function<AbstractAsteroidObject* (Transform*)>;

class AsteroidFactory {
    private:
        static float specialPietrinoProbability;
        // chatGPT magic
        static std::vector<AsteroidConstructor>& getConstructors() {
            static std::vector<AsteroidConstructor> constructors;
            return constructors;
        }
    public:
        static void spawnRandomAsteroid(Scene* scene, Transform* target);

        template <class Asteroid>
        static void registerAsteroid();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#ifdef ASTEROIDFACTORY_IMPLEMENTATION
#undef ASTEROIDFACTORY_IMPLEMENTATION

// creates a list of asteroid constructors
template <class Asteroid>
void AsteroidFactory::registerAsteroid() {
    getConstructors().push_back([](Transform* t) -> AbstractAsteroidObject* { return new Asteroid(t); });
}

#endif