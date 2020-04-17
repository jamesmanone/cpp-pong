#include <memory>
#include <thread>
#include "interactive.h"
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"



class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  Renderer(Renderer &s) = delete;
  Renderer(Renderer &&s);
  ~Renderer();

  void AddDrawable(Interactive *i);
  void SetDrawable(std::vector<Interactive*> i);
  void ClearDrawable();

  void Render();
  void Stop() { _running = false; };

  void UpdateWindowTitle(int score, int fps);

  int getScreenWidth() const { return (int)screen_width; }
  int getScreenHeight() const { return (int)screen_height; }

 private:
  void _render();
  void DrawRect(Location l, Color c);
  void DrawCircle(Location l, Color c);

  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  std::vector<Interactive*> _drawable;
  std::vector<std::thread> _thread;
  bool _running{true};


  const std::size_t screen_width;
  const std::size_t screen_height;
};

#endif