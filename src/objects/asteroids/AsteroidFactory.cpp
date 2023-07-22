#include "../../../headers/engine/base/Random.hpp"
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
#include "../../../headers/objects/asteroids/Pietrino.hpp"
#include "../../../headers/objects/asteroids/Pietro.hpp"

//std::vector<AsteroidConstructor> AsteroidFactory::constructors;
float AsteroidFactory::specialPietrinoProbability = 0.0f; // probability to spawn a special pietrino in (0, 1)

void AsteroidFactory::spawnRandomAsteroid(Scene* scene, Transform* target) {
	// list of constructors
	auto constructors = getConstructors();
	bool spawnSpecialPietrino = Random::randomFloat(0, 1) < specialPietrinoProbability;

	if (!spawnSpecialPietrino && constructors.size() > 0) {
		// spawn normal asteroid
		int asteroidChoice = Random::randomInt(0 , constructors.size());
		AbstractAsteroidObject* asteroid = constructors[asteroidChoice](target);
		asteroid->Instantiate();
		scene->addObject(asteroid);
	}
	else {

	}
}