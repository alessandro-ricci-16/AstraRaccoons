#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_

#include "includes.hpp"

class Random {
	private:
		static float randomFloat();
	public:
		static int randomInt(int a, int b);
		static float randomFloat(float a, float b);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_