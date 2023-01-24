#include "game.h"
#include <iostream>
#include "SDL.h"


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, snakeFood);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

enum Food { snack, poison, booster };

int poisonTimerStart = 0;
int poisonTimerEnd = 0;
void Game::setFood(int& x, int& y)
{
      
  std::random_device ldev;   
  std::mt19937 rand_no(ldev());   
  std::uniform_int_distribution<int> tempFood(0,2); 
  bool tmpRet = false;
  auto random_FeedIndex = tempFood(rand_no);

  switch((Food)random_FeedIndex){
    case booster:
       snakeFood = SnakeBooster(x,y);
      break;
    case poison:
      snakeFood = SnakePoison(x,y);
      poisonTimerStart = SDL_GetTicks(); // keep the posion for 3 seconds
      break;
    case snack:
      snakeFood = SnakeSnacks(x,y);
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      setFood(x,y);
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);


  if((snakeFood.getVal() == 0)) // current food is posion.
  {
    poisonTimerEnd = SDL_GetTicks();
  }
   // Check if there's food over here
  if ((snakeFood.getFoodXPos() == new_x && snakeFood.getFoodYPos() == new_y )) {
    if(snakeFood.getVal() == 0)
    {
      snake.alive = false;
      return;        
    }

    score += snakeFood.getVal(); 
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
// check if 3 seconds already passed for the posion.
  else if ((snakeFood.getVal() == 0) && ((poisonTimerEnd - poisonTimerStart) >= 3000))
  {
    poisonTimerStart = 0;
    poisonTimerEnd = 0;
    PlaceFood();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }