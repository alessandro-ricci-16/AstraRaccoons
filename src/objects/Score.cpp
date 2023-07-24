#include "../../headers/objects/Score.hpp"

void Score::addScore(int points) {
    score += points;
}

int Score::getScore() {
    return score;
}

void Score::resetScore() {
    score = 0;
}