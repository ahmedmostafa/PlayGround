#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include<string>

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height), head_x(0.5*grid_width), head_y(0.5*grid_height / 2) {}

  void Update();

  void GrowBody();
  bool SnakeCell(const int &x, const int &y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

class SnakeFood
{
public:
    SnakeFood();
    // Constructor & destructor
    SnakeFood(int posX, int posY)
        : x_(posX), y_(posY), color_({153, 153, 255}), val_(1){}

    // Overloaded constructor
    SnakeFood(int posX, int posY, std::vector<int> foodColor, int val)
        : x_(posX), y_(posY), color_(foodColor), val_(val) {}

    void setFoodXPos(int x) { x_ = x; }
    int getFoodXPos() { return x_; }

    void setFoodYPos(int y) { y_ = y; }
    int getFoodYPos() { return y_; }

    void setVal(int v) { val_ = v; }
    int getVal() { return val_; }

    void setColor(std::vector<int> c) { color_ = c; }
    std::vector<int> getColor() { return color_; }

    int x_{0};
    int y_{0};
    int val_{1};
    std::vector<int> color_ = {153, 153, 255};

};


class SnakeSnacks : public SnakeFood
{
public:
    SnakeSnacks(int x, int y) : SnakeFood(x, y, {153, 153, 255}, 1) {}
};

class SnakeBooster : public SnakeFood
{
public:
    SnakeBooster(int x, int y) : SnakeFood(x, y, {0,255,0}, 10) {}
};


class SnakePoison : public SnakeFood
{
public:
    SnakePoison(int x, int y) : SnakeFood(x, y, {255,0,0}, 0) {}  
};

#endif