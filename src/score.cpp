#include "score.h"
#include <fstream>
#include <algorithm>

Score::Score()
    :highScore(0)
{

}

int Score::getHighScore()
{
    return highScore;
}

void Score::SaveScore(int score)
{
    ofstream _boardFile;
   _boardFile.open("ScoreBoard.txt", fstream::app);
   _boardFile << "Player_Score: " << score << "\n";
   _boardFile.close();
}

int Score::updateHighScore()
{
    ifstream _scoreBoard;
    string line;
    string key;
    int value;

    _scoreBoard.open("ScoreBoard.txt");
    if(_scoreBoard.is_open())
    {
        while(std::getline(_scoreBoard, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "Player_Score:")
            {
                if(value >= highScore)
                {
                    highScore = value;
                }
            }
        }
    }
    _scoreBoard.close();
    return highScore;
}

