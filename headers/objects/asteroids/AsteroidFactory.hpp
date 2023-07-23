#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#include "../../engine/scenes/Scene.hpp"
#include "../../objects/asteroids/AbstractAsteroidObject.hpp"
#include "../../engine/base/includes.hpp"

struct AsteroidConstructor {
    std::function<AbstractAsteroidObject*(GameObject*)> constructor;
    int id;
};

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
        static void registerSpecialAsteroid(bool oneTime = false);
        template <class Asteroid>
        static void unregisterSpecialAsteroid(int constructorID);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_ASTEROIDS_FACTORYOBJECT_HPP_

#ifdef ASTEROIDFACTORY_IMPLEMENTATION
#undef ASTEROIDFACTORY_IMPLEMENTATION

// creates a list of normal asteroid constructors
template <class Asteroid>
void AsteroidFactory::registerAsteroid() {
    int constructorID = Random::randomInt(0, INT_MAX);
    getConstructors().push_back({[](GameObject* g) -> AbstractAsteroidObject* { return new Asteroid(g); }, constructorID});
}

// creates a list of special asteroid constructors
template <class Asteroid>
void AsteroidFactory::registerSpecialAsteroid(bool oneTime) {
    int constructorID = Random::randomInt(0, INT_MAX);
    getSpecialConstructors().push_back({[constructorID, oneTime](GameObject* g) -> AbstractAsteroidObject* {
        if (oneTime) {
            AsteroidFactory::unregisterSpecialAsteroid<Asteroid>(constructorID);
        }
        return new Asteroid(g);
    }, constructorID});
}

template <class Asteroid>
void AsteroidFactory::unregisterSpecialAsteroid(int constructorID) {
    for (int i = 0; i < getSpecialConstructors().size(); i++) {
        auto constructor = getSpecialConstructors()[i];
        if (constructor.id == constructorID) {
            getSpecialConstructors().erase(getSpecialConstructors().begin() + i);
            break;
        }
    }
}

#endif