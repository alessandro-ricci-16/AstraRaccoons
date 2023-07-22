#include <headers/engine/base/Random.hpp>

void Random::initialize() {
	srand(time(NULL));
}

float Random::randomFloat() {
	return (float)(rand()) / (float)(RAND_MAX);
}

// in range [a, b)
int Random::randomInt(int a, int b) {
	if (a > b) return randomInt(b, a);
	if (a == b) return a;
	return a + (rand() % (b - a));
}

float Random::randomFloat(float a, float b) {
	if (a > b) return randomFloat(b, a);
	if (a == b) return a;

	return randomFloat() * (b - a) + a;
}