#include "score.h"
#include <fstream>
#include <algorithm>
#include <vector>

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
   _boardFile.open("scores.txt", fstream::app);
   _boardFile << "Player_Score: " << score << "\n";
   _boardFile.close();
}

int Score::getLastHighestScore()
{
    ifstream fSavedScores;
    string line;
    string key;
    int value;
    int hScore{0};
    std::vector<int> savedScores;

    fSavedScores.open("scores.txt");
    if(fSavedScores.is_open())
    {
        while(std::getline(fSavedScores, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "Player_Score:")
            {
                savedScores.push_back(value);
            }
        }
        hScore = *std::max_element(savedScores.begin(), savedScores.end());
        fSavedScores.close();
    } 
    highScore = hScore;     
    return highScore;
}




int Score::updateHighScore()
{
    ifstream fSavedScores;
    string line;
    string key;
    int value;

    fSavedScores.open("scores.txt");
    if(fSavedScores.is_open())
    {
        while(std::getline(fSavedScores, line))
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
    fSavedScores.close();
    return highScore;
}

