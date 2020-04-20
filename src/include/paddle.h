#ifndef PADDLE_H
#define PADDLE_H
#include "interactive.h"


class Paddle : public Interactive {
public:
  enum class Direction { kUp, kDown, kNone };
  enum class Type { kPlayer, kComputer };
  void move(Direction d) { _moving = d; }
  // virtual void Act();
protected:
  Paddle(Color c, Location l, Game *g) : Interactive(c, l, g) { }
  Direction _moving{Direction::kNone};
  void _move() override;
  static constexpr int _width{20};
  static constexpr int _height{75};
  static constexpr double delta_y{0.27};
};

#endif