#include "SDL.h"
#include "color.h"
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
// #include "game.h"

#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "drawable.h"

// Forward Declaration
class Game;

class Interactive : public Drawable {
public:
  static void setXY(int x, int y);
  Interactive() { }
  virtual ~Interactive() {
    Stop();
    for(auto &t : _threads) t.join();
  }

  virtual void Stop() { _running = false; }

  void setGame(Game *g) { _game = g; }

  virtual void Act() = 0;
protected:
  Interactive(Color c) : Drawable(c) { }
  Interactive(Color c, Location l) : Drawable(c, l) { }
  Interactive(Color c, Location l, Game *g) : Drawable(c, l), _game(g) { }

  virtual void _move() = 0;


  std::chrono::high_resolution_clock::time_point _last;
  std::vector<std::thread> _threads;
  Game *_game;
  bool _running{true};
  std::mutex _mtx;

  static constexpr double maxX{1};
  static constexpr double maxY{1};
};

#endif