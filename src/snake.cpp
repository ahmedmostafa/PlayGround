#include <iostream>
#include "snake.hpp"

void Snake::Update()
{
  //Captures the position of the head prior to updating it
  SDL_Point prev_cell
  {
    static_cast<int>(m_head_x),
    static_cast<int>(m_head_y)
  };
  UpdateHead();
  //Captures the position of the head after updating it
  SDL_Point current_cell
  {
    static_cast<int>(m_head_x),
    static_cast<int>(m_head_y)
  };

  //If the snake's head has moved to a new cell, it updates the body vector
  if(current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
  {
    UpdateBody(current_cell, prev_cell);
  }
}

//Updates the head position according to the direction of the snake
void Snake::UpdateHead()
{
  switch(direction)
  {
    case Direction::kUp:
      m_head_y -= m_speed;
      break;

    case Direction::kDown:
      m_head_y += m_speed;
      break;

    case Direction::kLeft:
      m_head_x -= m_speed;
      break;

    case Direction::kRight:
      m_head_x += m_speed;
      break;
  }

  //Kills the snake if the head goes beyond the limits of the grid
  if(((int)(m_grid_width - m_head_x) | (int)(m_head_x - 0) | (int)(m_grid_height - m_head_y) | (int)(m_head_y - 0)) <= 0)
  {
    losesLive();
  }
}

//Updates the body of the snake according to the position of the head
void Snake::UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell)
{
  //Adds the previous head location to the vector
  body.emplace_back(prev_cell);

  if(!m_growing)
  {
    body.erase(body.begin());
  } else {
    m_growing = false;
    m_size++;
  }

  //Checks if the snake has died
  for(auto const &item : body)
  {
    if (current_cell.x == item.x && current_cell.y == item.y)
    {
      m_alive = false;
    }
  }
}

void Snake::GrowBody()
{
  m_growing = true;
}

void Snake::shrinkSnake()
{
  body.pop_back();
}

bool Snake::SnakeCell(int x, int y)
{
  if(x == static_cast<int>(m_head_x) && y == static_cast<int>(m_head_y))
  {
    return true;
  }

  for(auto const &item : body)
  {
    if(x == item.x && y == item.y)
    {
      return true;
    }
  }

  return false;
}

//Resets snake
void Snake::resetSnake()
{
  m_size = 1;
  m_speed = 0.1f;
  m_alive = true;
  m_head_x = m_grid_width/2;
  m_head_y = m_grid_height/2;
  m_lives = 3;
  m_reset = false;
  body.clear();
}

//Adds an extra live to the Snake, for a maximum of 4
void Snake::gainsLive()
{
  if(m_lives < 4)
  {
    m_lives += 1;
  }
}

//Loses a live
void Snake::losesLive()
{
  if(m_lives > 0)
  {
    m_lives -= 1;
  } else {
    m_alive = false;
  }
}
