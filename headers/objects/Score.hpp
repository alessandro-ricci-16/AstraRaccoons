#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_

#include "../engine/base/includes.hpp"

class Score {
private:
    int score = 0;
public:
    void addScore(int points);
    int getScore();
    void resetScore();
};

#endif //__DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_OBJECTS_SCORE_HPP_