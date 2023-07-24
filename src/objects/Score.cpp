#include "../../headers/objects/Score.hpp"

int Score::score = 0;

void Score::addScore(int points) {
    score += points;
}

int Score::getScore() {
    return score;
}

void Score::resetScore() {
    score = 0;
}