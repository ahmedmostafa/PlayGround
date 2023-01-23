#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <string>

#include <SDL_ttf.h>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height,
           const std::size_t borderwidth, const std::size_t borderheight);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, 
              SDL_Point const &obstacle, const int score);
  void UpdateWindowTitle(int score, int fps);
  void DrawBorder();
  bool GetInit();

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  SDL_Surface* snake_surfc;
  SDL_Texture* image_head;
  SDL_Surface* fire_surfc;
  SDL_Texture* image_fire;
  SDL_Surface* food_surfc;
  SDL_Texture* image_food;    
      
  TTF_Font* font;
  std::string title;
  SDL_Surface* textsurface;
  SDL_Texture* texture;
  bool bInit;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  const std::size_t border_width;
  const std::size_t border_height;

  void DrawText(std::string& title, bool middle, SDL_Rect& block);
};

#endif