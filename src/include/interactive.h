#include "game.h"
#include "SDL.h"
#include "color.h"
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "location.h"

// Forward Declaration
class Game;

class Interactive {
public:
  static void setXY(int x, int y);
  Interactive() { }
  virtual ~Interactive();

  static void SetMax(int x, int y) { maxX = x; maxY = y; }

  void setGame(Game *g) { _game = g; }
  Color getColor() const { return _color; }
  Location getLocation() const { return _location; }
  int PosX() const { return _location.X(); }
  int PosY() const  { return _location.Y(); }

  virtual void Act() = 0;
protected:
  Interactive(Color c) : _color(c) { }
  Interactive(Color c, Location l) : _color(c), _location(l) { }
  Interactive(Color c, Location l, Game *g) : _color(c), _location(l), _game(g) { }

  virtual void _move() = 0;


  Location _location;
  std::chrono::high_resolution_clock::time_point _last;
  Color _color;
  std::vector<std::thread> _thread;
  Game* _game;

  static int maxX;
  static int maxY;
};

#endif