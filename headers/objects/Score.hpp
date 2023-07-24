#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_

#include "../engine/base/includes.hpp"

class Score {
private:
    static int score;
public:
    static void addScore(int points);
    static int getScore();
    static void resetScore();
};

#endif //__DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_