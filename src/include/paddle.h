#ifndef PADDLE_H
#define PADDLE_H
#include "interactive.h"


class Paddle : public Interactive {
public:
  enum class Direction { kUp, kDown, kNone };
  enum class Type { kComputer = 0, kPlayer };
  void move(Direction d) { _moving = d; }
protected:
  Paddle(Color c, Location l, Game *g) : Interactive(c, l, g) { }
  Direction _moving{Direction::kNone};
  void _move() override;
  static constexpr float _width{0.01};
  static constexpr float _height{0.1};
  static constexpr double delta_y{0.0004};
};

#endif