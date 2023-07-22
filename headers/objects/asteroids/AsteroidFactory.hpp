#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#include "../../engine/scenes/Scene.hpp"
#include "../../objects/asteroids/AbstractAsteroidObject.hpp"
#include "../../engine/base/includes.hpp"

using AsteroidConstructor = std::function<AbstractAsteroidObject* (GameObject*)>;

class AsteroidFactory {
    private:
        static float specialPietrinoProbability;

        // chatGPT magic
        static std::vector<AsteroidConstructor>& getConstructors() {
            static std::vector<AsteroidConstructor> constructors;
            return constructors;
        }
        static std::vector<AsteroidConstructor>& getSpecialConstructors() {
            static std::vector<AsteroidConstructor> specialConstructors;
            return specialConstructors;
        }

    public:
        static void spawnRandomAsteroid(Scene* scene, GameObject* player);

        template <class Asteroid>
        static void registerAsteroid();
        template <class Asteroid>
        static void registerSpecialAsteroid();
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#ifdef ASTEROIDFACTORY_IMPLEMENTATION
#undef ASTEROIDFACTORY_IMPLEMENTATION

// creates a list of normal asteroid constructors
template <class Asteroid>
void AsteroidFactory::registerAsteroid() {
    getConstructors().push_back([](GameObject* g) -> AbstractAsteroidObject* { return new Asteroid(g); });
}

// creates a list of special asteroid constructors
template <class Asteroid>
void AsteroidFactory::registerSpecialAsteroid() {
    getSpecialConstructors().push_back([](GameObject* g) -> AbstractAsteroidObject* { return new Asteroid(g); });
}

#endif