#include "../../../headers/engine/base/Random.hpp"
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
#include "../../../headers/objects/asteroids/Pietrino.hpp"
#include "../../../headers/objects/asteroids/Pietro.hpp"

//std::vector<AsteroidConstructor> AsteroidFactory::constructors;
float AsteroidFactory::specialPietrinoProbability = 0.2f; // probability to spawn a special pietrino in (0, 1)

void AsteroidFactory::spawnRandomAsteroid(Scene* scene, GameObject* player) {
	// list of constructors
	auto constructors = getConstructors();
	auto specialConstructors = getSpecialConstructors();
	bool spawnSpecialPietrino = Random::randomFloat(0, 1) < specialPietrinoProbability;
	AbstractAsteroidObject* asteroid = nullptr;

	if (!spawnSpecialPietrino && constructors.size() > 0) {
		// spawn normal asteroid
		int asteroidChoice = Random::randomInt(0, (int)constructors.size());
		asteroid = constructors[asteroidChoice].constructor(player);
	}
	else if (specialConstructors.size() > 0){
		// spawn special asteroid
		int asteroidChoice = Random::randomInt(0, (int)specialConstructors.size());
		asteroid = specialConstructors[asteroidChoice].constructor(player);
	}
	else
		std::cerr << "No asteroid constructor found\n";

	if (asteroid != nullptr) {
		asteroid->Instantiate();
		scene->addObject(asteroid);
	}
}
