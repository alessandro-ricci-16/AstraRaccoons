#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_

#include "includes.hpp"

class Random {
    public:
        static float randomFloat();
        static int randomInt(int a, int b);
        static float randomFloat(int a, int b);
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_ENGINE_BASE_RANDOM_HPP_