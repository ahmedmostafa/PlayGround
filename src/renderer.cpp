#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height,
                   const std::size_t borderwidth, const std::size_t borderheight)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      border_width(borderwidth),
      border_height(borderheight){
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() == -1)
  {
      printf("TTF_Init: %s\n", TTF_GetError());
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  
  std::string sPath = SDL_GetBasePath();
  sPath += "../Resources/arial.ttf";
  //Draw text
  font = TTF_OpenFont(sPath.c_str(), 20);
  
  sPath = SDL_GetBasePath();
  sPath += "../Resources/snake.bmp";
  snake_surfc = SDL_LoadBMP(sPath.c_str());
  image_head = SDL_CreateTextureFromSurface(sdl_renderer, snake_surfc);

  sPath = SDL_GetBasePath();
  sPath += "../Resources/fire.bmp";
  fire_surfc = SDL_LoadBMP(sPath.c_str());
  image_fire = SDL_CreateTextureFromSurface(sdl_renderer, fire_surfc);

  sPath = SDL_GetBasePath();
  sPath += "../Resources/food.bmp";
  food_surfc = SDL_LoadBMP(sPath.c_str());
  image_food = SDL_CreateTextureFromSurface(sdl_renderer, food_surfc);

  bInit = false;
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(textsurface);
  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyTexture(image_head);
  SDL_FreeSurface(snake_surfc);

  SDL_DestroyTexture(image_fire);
  SDL_FreeSurface(fire_surfc);

  SDL_DestroyTexture(image_food);
  SDL_FreeSurface(food_surfc);
  SDL_Quit();
}

void Renderer::DrawBorder()
{
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Render border
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF);
    //Vidhya: Assuming a square grid always.
    for (int i=0; i<grid_width; i++)
    {
        //Bottom border line
        block.x = i * block.w;
        block.y = 0;
        SDL_RenderFillRect(sdl_renderer, &block);

        //Top border line
        block.x = i * block.w;
        block.y = border_height;
        SDL_RenderFillRect(sdl_renderer, &block);

        //Left border line
        block.x = 0;
        block.y = i * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);

        //Right border line
        block.x = border_width;
        block.y = i * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }
}

void Renderer::Render(Snake const snake, SDL_Point const &food, 
                      SDL_Point const &obstacle, const int score) 
{
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderClear(sdl_renderer);

  if (!bInit)
  {
      std::string msg = "Welcome to Snake Game. This game has a snake that is in search of food. Move the snake to food using arrow keys. When the snake eats food "
          "it grows.There is also fire(obstacle) in the path. If the snake hits an obstacle or hit the border it will die."
          "\n\n                Best of luck!!!"
          "\n\n  Hit any key when you are ready.";

      DrawText(msg, true, block);
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
          switch (event.type)
          {
          case SDL_KEYDOWN:
              bInit = true;
              return;
          }
      }
  } 
  else
  {
      //Vidhya: Render border
      DrawBorder();

      // Render food
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
      block.x = food.x * block.w;
      block.y = food.y * block.h;
      //SDL_RenderFillRect(sdl_renderer, &block);
      SDL_RenderCopy(sdl_renderer, image_food, NULL, &block);

      //Vidhya: Render obstacle
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
      block.x = obstacle.x * block.w;
      block.y = obstacle.y * block.h;
      //SDL_RenderFillRect(sdl_renderer, &block);
      SDL_RenderCopy(sdl_renderer, image_fire, NULL, &block);

      // Render snake's body
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      for (SDL_Point const& point : snake.body) {
          block.x = point.x * block.w;
          block.y = point.y * block.h;
          //SDL_RenderFillRect(sdl_renderer, &block);
          SDL_RenderCopy(sdl_renderer, image_head, NULL, &block);
      }

      // Render snake's head
      block.x = static_cast<int>(snake.head_x) * block.w;
      block.y = static_cast<int>(snake.head_y) * block.h;
      if (snake.alive) {
          //Render snake image
          SDL_RenderCopy(sdl_renderer, image_head, NULL, &block);

          //Update title score
          title = "Snake Score: " + std::to_string(score);
          DrawText(title, false, block);
      }
      else
      {
          //Snake dead. Show the final message.
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
          if (score > 10)
              title = "Congrats!!!! \nYou Scored " + std::to_string(score);
          else
              title = "Better luck next time!!!! \nYou Scored " + std::to_string(score);
          DrawText(title, true, block);
      }
  }
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::DrawText(std::string& title, bool middle, SDL_Rect& block)
{
    int texW = 30;
    int texH = 30;
    SDL_Color color = { 255, 255, 255 };

    if (middle)
    {
        color.g = 0;
        color.b = 0;
    }
    //textsurface = TTF_RenderText_Solid(font,
   //     title.c_str(), color);
    textsurface = TTF_RenderText_Blended_Wrapped(font, title.c_str(), color, 300);

    texture = SDL_CreateTextureFromSurface(sdl_renderer, textsurface);

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 0, 0, texW, texH };
    if (middle)
    {
        dstrect.x = (screen_width / 2)-(block.w*5);
        dstrect.y = (screen_height / 2)-(block.h*2);
    }

    SDL_RenderCopy(sdl_renderer, texture, NULL, &dstrect);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

bool Renderer::GetInit()
{
    return bInit;
}
