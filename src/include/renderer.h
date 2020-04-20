#include <memory>
#include <thread>
#include <mutex>
#include "drawable.h"
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
class Game;



class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  Renderer(Renderer &s) = delete;
  Renderer(Renderer &&s);
  ~Renderer();

  void AddDrawable(std::shared_ptr<Drawable> i) { _drawable.emplace_back(i); }
  void SetDrawable(std::vector<std::shared_ptr<Drawable>> i);
  void ClearDrawable();
  void SetGamePointer(Game *g) { _game = g; }
  void SetScoreFlag() { _newScore = true; }

  void Render();
  void Stop() { _running = false; };

  void UpdateWindowTitle();

  int getScreenWidth() const { return (int)screen_width; }
  int getScreenHeight() const { return (int)screen_height; }

 private:
  void _render();
  void DrawRect(Location l, Color c);
  void DrawCircle(Location l, Color c);

  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  std::vector<std::shared_ptr<Drawable>> _drawable;
  std::vector<std::thread> _thread;
  Game *_game;
  bool _running{true};
  bool _newScore{false};
  std::mutex _mtx;
  int _frames{0};

  const std::size_t screen_width;
  const std::size_t screen_height;
};

#endif