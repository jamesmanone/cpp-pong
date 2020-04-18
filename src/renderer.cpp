#include "include/renderer.h"
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
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
  // _thread.emplace_back(std::thread(&Renderer::UpdateWindowTitle, this));
  _render();
}

void Renderer::_render() {
  while(_running) {
    std::unique_lock<std::mutex> l(_mtx);
    ++_frames;
    l.unlock();

    SDL_Event e;
    if(SDL_PollEvent(&e) && e.type == SDL_QUIT) _running = false;

    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x05, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer);

    // Render all objects

    for(auto draw : _drawable) {
      Location l = draw->getLocation();
      Color c = draw->getColor();

      if(l.H() == 0) DrawCircle(l, c);  // circle
      else DrawRect(l, c);
    }

    SDL_RenderPresent(sdl_renderer);
    // SDL_Delay(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

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


void Renderer::DrawCircle(Location l, Color c) {
  int c_x = l.X(),
      c_y = l.Y(),
      r = l.W()/2;
    
    SDL_SetRenderDrawColor(sdl_renderer, c.R(), c.G(), c.B(), c.A());

    // Solution borrowed from github gist https://gist.github.com/derofim/912cfc9161269336f722
    for(double dy = 1; dy <= r; dy += 1) {
      double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));

      SDL_RenderDrawLine(sdl_renderer, c_x - dx, c_y + dy - r, c_x + dx, c_y + dy - r);
      SDL_RenderDrawLine(sdl_renderer, c_x - dx, c_y - dy + r, c_x + dx, c_y - dy + r);
    }
}

void Renderer::UpdateWindowTitle() {
  std::chrono::high_resolution_clock::time_point last(std::chrono::high_resolution_clock::now());
  while(_running) {
  
    std::unique_lock<std::mutex> l(_mtx);
    auto now = std::chrono::high_resolution_clock::now();
    int f = _frames;
    _frames = 0;
    l.unlock();

    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    last = now;

    int fps = d == 0 ? 0 : f / (d * 1000);

    std::cout << "\rFPS: " << fps;
    // std::string title{"Pong -   FPS: " + std::to_string(fps)};
    // SDL_SetWindowTitle(sdl_window, title.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }
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