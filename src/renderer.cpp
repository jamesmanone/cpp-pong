#include "include/renderer.h"
#include <iostream>
#include <string>
#include "include/color.h"

Renderer::Renderer(Renderer &&s) :
                  screen_height(s.screen_height), screen_width(s.screen_width) {
  sdl_window = std::move(s.sdl_window);
  s.sdl_window = nullptr;
  sdl_renderer = std::move(s.sdl_renderer);
  s.sdl_renderer = nullptr;

  _drawable = s._drawable;

  _thread = std::move(s._thread);
}


Renderer::Renderer(const std::size_t screen_width,
                  const std::size_t screen_height) :
                  screen_width(screen_width),
                  screen_height(screen_height) {
  // Initialize SDL

  std::cout << "Resolution: " << screen_width << "x" << screen_height << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  } else std::cout << "Init\n";

  // Create Window
  sdl_window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, 0);

  if (nullptr == sdl_window || sdl_window == NULL) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  } else std::cout << "Window aquired at " << sdl_window << std::endl;

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);


  if (nullptr == sdl_renderer || sdl_renderer == NULL) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  } else std::cout << "Renderer aquired\n";
}

Renderer::~Renderer() {
  std::cout << "Renderer destructor\n";
  for(auto &th : _thread) th.join();
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render() {
  // _thread.emplace_back(std::thread(&Renderer::_render, this));
  _render();
}

void Renderer::_render() {
  while(_running) {
    SDL_Event e;
    if(SDL_PollEvent(&e) && e.type == SDL_QUIT) break;

    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x05, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer);

    // Render all objects

    for(auto draw : _drawable) {
      Location l = draw->getLocation();
      Color c = draw->getColor();

      // if(l.H() == 0) DrawCircle(l, c);  // circle
      DrawRect(l, c);
    }

    SDL_RenderPresent(sdl_renderer);
    // SDL_Delay(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }


  // Render food
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  // block.x = food.x * block.w;
  // block.y = food.y * block.h;
  // SDL_RenderFillRect(sdl_renderer, &block);

  // // Render snake's body
  // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  // for (SDL_Point const &point : snake.body) {
  //   block.x = point.x * block.w;
  //   block.y = point.y * block.h;
  //   SDL_RenderFillRect(sdl_renderer, &block);
  // }

  // // Render snake's head
  // block.x = static_cast<int>(snake.head_x) * block.w;
  // block.y = static_cast<int>(snake.head_y) * block.h;
  // if (snake.alive) {
  //   SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  // } else {
  //   SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  // }
  // SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  // SDL_RenderPresent(sdl_renderer);
}

void Renderer::DrawRect(Location l, Color c) {
  SDL_Rect rect;
  rect.w = l.W();
  rect.h = l.H();
  rect.x = l.X();
  rect.y = l.Y();
  if(rect.h == 0) rect.h = rect.w;

  SDL_SetRenderDrawColor(sdl_renderer, c.R(), c.G(), c.B(), c.A());
  SDL_RenderFillRect(sdl_renderer, &rect);
}

// Circle drawing algo adapted from https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl (2nd answer)
void Renderer::DrawCircle(Location l, Color c) {
  int c_x = l.X(),
      c_y = l.Y(),
      r = l.W()/2,
      diameter = r*2,
      x = r-1, y = 0,
      tx = 1,
      ty = 1,
      e = tx-diameter;

  while(x >= y) {
    SDL_RenderDrawPoint(sdl_renderer, c_x + x, c_y - y);
    SDL_RenderDrawPoint(sdl_renderer, c_x + x, c_y + y);
    SDL_RenderDrawPoint(sdl_renderer, c_x - x, c_y - y);
    SDL_RenderDrawPoint(sdl_renderer, c_x - x, c_y + y);
    SDL_RenderDrawPoint(sdl_renderer, c_x + y, c_y - x);
    SDL_RenderDrawPoint(sdl_renderer, c_x + y, c_y + x);
    SDL_RenderDrawPoint(sdl_renderer, c_x - y, c_y - x);
    SDL_RenderDrawPoint(sdl_renderer, c_x - y, c_y + x);

    if (e <= 0)
      {
         ++y;
         e += ty;
         ty += 2;
      }

      if (e > 0)
      {
         --x;
         tx += 2;
         e += (tx - diameter);
      }
  }
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Pong " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::SetDrawable(std::vector<Interactive*> i) {
  _drawable.clear();
  for(auto p : i) _drawable.emplace_back(p);
  std::cout << _drawable.front()->getLocation().X() << std::endl;
}

// pointeres to drawable objects are not owned
void Renderer::ClearDrawable() {
  _drawable.clear();
}