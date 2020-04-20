#include "include/renderer.h"
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include "include/color.h"
#include "include/game.h"

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


  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_RESIZABLE);

  if (nullptr == sdl_window || sdl_window == NULL) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);


  if (nullptr == sdl_renderer || sdl_renderer == NULL) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  for(auto &th : _thread) th.join();
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render() {
  _render();  // All rendering MUST be on main thread
}

void Renderer::_render() {
  while(_running) {

    SDL_Event e;
    if(SDL_PollEvent(&e)) _game->Send(std::move(e));
    if(_newScore) UpdateWindowTitle();

    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x05, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer);

    // Render all objects

    std::unique_lock<std::mutex> l(_mtx);
    for(auto draw : _drawable) {
      Location l = draw->getLocation();
      Color c = draw->getColor();

      if(l.H() == 0.0) DrawCircle(l, c);  // circle
      else DrawRect(l, c);
    }
    l.unlock();
    SDL_RenderPresent(sdl_renderer);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

}

void Renderer::DrawRect(Location l, Color c) {
  SDL_Rect rect;
  rect.w = l.W() * screen_width;
  rect.h = l.H() * screen_height;
  rect.x = l.X() * screen_width;
  rect.y = l.Y() * screen_height;
  if(rect.h == 0) rect.h = rect.w;

  SDL_SetRenderDrawColor(sdl_renderer, c.R(), c.G(), c.B(), c.A());
  SDL_RenderFillRect(sdl_renderer, &rect);
}


void Renderer::DrawCircle(Location l, Color c) {
  int c_x = l.X() * screen_width,
      c_y = l.Y() * screen_height,
      r = (l.W() * screen_width)/2;
    
    SDL_SetRenderDrawColor(sdl_renderer, c.R(), c.G(), c.B(), c.A());

    // Solution borrowed from github gist https://gist.github.com/derofim/912cfc9161269336f722
    for(double dy = 1; dy <= r; dy += 1) {
      double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));

      SDL_RenderDrawLine(sdl_renderer, c_x - dx, c_y + dy - r, c_x + dx, c_y + dy - r);
      SDL_RenderDrawLine(sdl_renderer, c_x - dx, c_y - dy + r, c_x + dx, c_y - dy + r);
    }
}

void Renderer::UpdateWindowTitle() {
  _newScore = false;
  char title[101];
  for(auto &p : title) p = ' ';
  title[100] = 00;  // null terminator
  title[48] = 'P';
  title[49] = 'O';
  title[50] = 'N';
  title[51] = 'G';

  std::string p_s = std::to_string(_game->PlayerScore());
  std::string c_s = std::to_string(_game->ComputerScore());

  if(p_s.size() == 1) title[1] = p_s[0];
  else memcpy(&title, p_s.c_str(), p_s.size());

  if(c_s.size() == 1) title[99] = c_s[0];
  else if(c_s.size() == 2) memcpy(&title[98], c_s.c_str(), 2);
  else memcpy(&title[97], c_s.c_str(), 3);

  SDL_SetWindowTitle(sdl_window, title);
}

void Renderer::SetDrawable(std::vector<std::shared_ptr<Drawable>> i) {
  ClearDrawable();
  std::lock_guard<std::mutex> l(_mtx);
  for(auto p : i) _drawable.emplace_back(p);
}


void Renderer::ClearDrawable() {
  std::lock_guard<std::mutex> l(_mtx);
  _drawable.clear();
}