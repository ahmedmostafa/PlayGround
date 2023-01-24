#ifndef SCORE_H
#define SCORE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace  std;

class Score
{
public:
    Score();
    void SaveScore(int score);
    int getHighScore();
    int updateHighScore();
    int getLastHighestScore();

private:
    int highScore{0};
};

#endif
